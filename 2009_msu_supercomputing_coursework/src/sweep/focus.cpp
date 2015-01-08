#include "mpi.h"
#include <complex>
#include <string>
#include <cstdlib>
#include <cstdio>
#include "initialization.cpp"
#include "finalization.cpp"
#include "propagation.cpp"

using namespace std;

//==========//============//==========//==========//==========//

#define HOST_NAME "Skif"

complex<double> _COMPLEX_I(0., 1.);

char mode;															// 'n' || 's' || 'a'
char dir_name[1024];
char init_file_name[1024];

int gsizes1[2] = {0, 0};											// array (Nrows, Ncolumns) of points in central uniform part
int gsizes2[2] = {0, 0};											// array (Nrows, Ncolumns) of points in border part (both sides)
int gsizes[2] = {gsizes1[0] + gsizes2[0], gsizes1[1] + gsizes2[1]};	// array (Nrows, Ncolumns) of points, rectangular grid
double al = 5.0;													// central area (gsizes1) in diameters of beam
double eps[2] = {0.05, 0.05};										// increment of step in border part of beam
double rad = 1.;													// radius of beam/pulse in sm, not diameter
double min_dxdy[2] = {0, 0};										// minimum of cross-section y-step of the grid, ~5 um

long int schmax = 1000;												// anyway, somewhere we must stop...
double dz = 1./1000.;												// initial step
double cur_z = 0.;													// distance passed
double PHASE_THRESHOLD_HIGH = 0.1;									// threshold for step decrement
double PHASE_THRESHOLD_LOW = 0.8*PHASE_THRESHOLD_HIGH;				// threshold for step increment

double kerrR = 0.;
complex<double> *EEE;												// EEE - pole of pulse part

double *dx, *dy;													// arrays with cross-section steps
complex<double> *sweep_a_x, *sweep_b_x, *sweep_c_x, *sweep_f_x;		// sweep coefficients for X-diffraction
complex<double> *sweep_a_y, *sweep_b_y, *sweep_c_y, *sweep_f_y;		// sweep coefficients for Y-diffraction
complex<double> *buf_x, *buf_y;										// arrays for forward-sweep coefficients
complex<double> *edge_l, *edge_r, *edge_t, *edge_b;					// arrays for pole from neighbour areas, needed for sweep_f
complex<double> *send_x, *send_y;									// arrays for sending neighbour pole

MPI_Datatype filetype_darray_complex, filetype_darray_double;		// filetype for block-distributed matrix
MPI_Comm comm_cart, comm_row, comm_col;								// cartesian communicator and communicators for row and column
int rank, size; 													// rank of process and size of communicator
int rank_row, rank_col;
int psizes[2] = {2, 0};												// sizes of communicator grid
int local_sizes[2];													// number of rows and columns on local process

char log_name[1024];												// name of .log file
FILE *p_log;

//==========//============//==========//==========//==========//

void MakeDirectories()
{
	char buf[1024];
	sprintf(buf, "mkdir -p %s", dir_name);
	system(buf);
}

void CreateComm()
{
	int periods[2] = {0, 0};
	int rem_dims_row[2] = {0, 1};
	int rem_dims_col[2] = {1, 0};
	
	MPI_Dims_create(size, 2, psizes);
	MPI_Cart_create(MPI_COMM_WORLD, 2, psizes, periods, 1, &comm_cart);
	MPI_Cart_sub(comm_cart, rem_dims_row, &comm_row);
	MPI_Cart_sub(comm_cart, rem_dims_col, &comm_col);

	MPI_Comm_rank(comm_cart, &rank);
	MPI_Comm_rank(comm_row, &rank_row);
	MPI_Comm_rank(comm_col, &rank_col);
}

void TestComm(MPI_Comm cart, char *name)
{
	int ndims;
	int *dims, *periods, *coords;
	
	p_log = fopen(log_name, "a+t");	

	MPI_Cartdim_get(cart, &ndims);
	fprintf(p_log, "%.2d: %s communicator\n	dimensions: %d\n", rank, name, ndims);
	dims = new int[ndims];
	periods = new int[ndims];
	coords = new int[ndims];
	MPI_Cart_get(cart, ndims, dims, periods, coords);
	fprintf(p_log, "	sizes: ");
	for (int i = 0; i < ndims; i++) fprintf(p_log, "%.2d ", dims[i]);
	fprintf(p_log, "\n	periods: ");
	for (int i = 0; i < ndims; i++) fprintf(p_log, "%.2d ", periods[i]);
	fprintf(p_log, "\n	coords: ");
	for (int i = 0; i < ndims; i++) fprintf(p_log, "%.2d ", coords[i]);
	fprintf(p_log, "\n\n");
	delete[] dims;
	delete[] periods;
	delete[] coords;
	
	fclose(p_log);
}

void OutParam()
{
	FILE *p_f;
	char buf[1024];
	
	sprintf(buf, "%s/param.txt", dir_name);
	p_f = fopen(buf, "wt+");
	
	fprintf(p_f, "X: uniform part    = %d\n", gsizes1[1]);
	fprintf(p_f, "   nonuniform part = %d\n", gsizes2[1]);
	fprintf(p_f, "   total           = %d\n", gsizes[1]);
	fprintf(p_f, "Y: uniform part    = %d\n", gsizes1[0]);
	fprintf(p_f, "   nonuniform part = %d\n", gsizes2[0]);
	fprintf(p_f, "   total           = %d\n", gsizes[0]);
	fprintf(p_f, "al = %f\n", al);
	fprintf(p_f, "eps = %f	%f\n", eps[0], eps[1]);
	fprintf(p_f, "rad = %f sm\n", rad);
	fprintf(p_f, "min_dx = %f um\n\n", min_dxdy[1]*10000);
	fprintf(p_f, "min_dy = %f um\n\n", min_dxdy[0]*10000);
	
	fprintf(p_f, "kerrR = %f\n", kerrR);
	
	fprintf(p_f, "Number of threads = %d\n", size);
	fprintf(p_f, "Cartesian communicator size = %d rows, %d columns\n", psizes[0], psizes[1]);
	
	fclose(p_f);
}

void TestParameters()
{
	if ((gsizes[0] % psizes[0] != 0) || (gsizes[1] % psizes[1] != 0))
	{
		if (rank == 0)
		{
			p_log = fopen(log_name, "a+t");
			fprintf(p_log, "001: Sizes of matrix and communicator grid don't match!\n");
			fclose(p_log);
		}
		MPI_Abort(comm_cart, 1);
	}
	else
	{
		local_sizes[0] = gsizes[0] / psizes[0];
		local_sizes[1] = gsizes[1] / psizes[1];
	}

	if ((gsizes2[0] % 2 != 0) || (gsizes2[1] % 2 != 0))
	{
		if (rank == 0)
		{
			p_log = fopen(log_name, "a+t");
			fprintf(p_log, "002: Odd number of border points!\n");
			fclose(p_log);
		}
		MPI_Abort(comm_cart, 2);
	}
}

void CreateFileType()
{
	int distribs[2], dargs[2];
	distribs[0] = MPI_DISTRIBUTE_BLOCK; 
	distribs[1] = MPI_DISTRIBUTE_BLOCK;
	dargs[0] = MPI_DISTRIBUTE_DFLT_DARG; 
	dargs[1] = MPI_DISTRIBUTE_DFLT_DARG;

	MPI_Type_create_darray(size, rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, MPI_DOUBLE_COMPLEX, &filetype_darray_complex);
	MPI_Type_commit(&filetype_darray_complex);
	MPI_Type_create_darray(size, rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, MPI_DOUBLE, &filetype_darray_double);
	MPI_Type_commit(&filetype_darray_double);
}

int main(int argc, char **argv)
{
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if (rank == 0)
	{
		gsizes1[0] = gsizes1[1] = atoi(argv[1]);
		kerrR = atof(argv[2]);
		mode = argv[3][0];
	}
	sprintf(init_file_name, "%s", argv[4]);
	MPI_Bcast(gsizes1, 2, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&kerrR, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&mode, 1, MPI_CHAR, 0, MPI_COMM_WORLD);
	gsizes[0] = gsizes1[0] + gsizes2[0];
	gsizes[1] = gsizes1[1] + gsizes2[1];
	min_dxdy[0] = rad*2*al/(gsizes1[0] - 1);
	min_dxdy[1] = rad*2*al/(gsizes1[1] - 1);
 	CreateComm();

	if (mode == 'n') sprintf(dir_name, "results/%s/Time_no_save/N%d_NP%d_R%lf", HOST_NAME, gsizes[0], size, kerrR);
	else if (mode == 's') sprintf(dir_name, "results/%s/Time_with_save/N%d_NP%d_R%lf", HOST_NAME, gsizes[0], size, kerrR);
	else if (mode == 'a') sprintf(dir_name, "results/%s/Accuracy/N%d_NP%d_R%lf", HOST_NAME, gsizes[0], size, kerrR);

	if (rank == 0) MakeDirectories();
	MPI_Barrier(MPI_COMM_WORLD);

	if (rank == 0)
	{
		sprintf(log_name, "%s/log.txt", dir_name);
		if ((p_log = fopen(log_name, "a+t")) == NULL) printf("Can't open file %s!\n", log_name);
		fprintf(p_log, "==============================================================\n");
		fprintf(p_log, "============   Propagation: Diffraction && Kerr   ============\n");
		fprintf(p_log, "==============================================================\n\n");
		fprintf(p_log, "MPI grid size: %d (%dx%d)\n\n", size, psizes[1], psizes[0]);
		fprintf(p_log, "N: %d\n", gsizes[1]);
		fprintf(p_log, "L: %.6f\n", al);
		fprintf(p_log, "Impulse file: %s\n\n", init_file_name);
		fprintf(p_log, "R: %f\n", kerrR);
		fprintf(p_log, "dz: %f (dphi < 0.1)\n", dz);
		fprintf(p_log, "Steps: %ld\n\n", schmax);
		fprintf(p_log, "n       dz         z          I_max(z)         P(z)\n");
		fclose(p_log);
	}
	TestParameters();
	CreateFileType();

	MPI_Barrier(MPI_COMM_WORLD);

	Allocate();
	Propagate();
	Finalize();

	MPI_Finalize();
}
