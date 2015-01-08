#include <ctime>
#include <cmath>
#include "output.cpp"

using namespace std;

//==========//============//==========//==========//==========//

extern char dir_name[1024];

extern int gsizes1[2], gsizes2[2], gsizes[2];
extern double al, eps[2], rad, min_dxdy[2], ldiff;

extern complex<double> *EEE;

extern double *dx, *dy;
extern complex<double> *sweep_a_x, *sweep_b_x, *sweep_c_x, *sweep_f_x;
extern complex<double> *sweep_a_y, *sweep_b_y, *sweep_c_y, *sweep_f_y;
extern complex<double> *buf_x, *buf_y;
extern complex<double> *edge_l, *edge_r, *edge_t, *edge_b;
extern complex<double> *send_x, *send_y;

extern int rank, size;
extern int rank_row, rank_col;
extern int psizes[2], local_sizes[2];
extern MPI_Comm comm_cart;

extern char log_name[1024];
extern FILE *p_log;

//==========//============//==========//==========//==========//

inline double step(int i, int N_central, int N_border, double e, double dstep_dimensionless)
{
	int tmp;
	if (i <= N_border/2 - 1)
	{
		tmp = N_border/2 - i;
		return dstep_dimensionless*pow(1 + e, tmp);
	}
	else
	if (i >= N_border/2 + N_central - 1)
	{
		tmp = i - N_border/2 - N_central + 2;
		return dstep_dimensionless*pow(1 + e, tmp);
	}
	else return dstep_dimensionless;
}

inline double coord_dimensionless(int i, int N_central, int N_border, double e)
{
	if (i < N_border/2) return -(N_central - 1)/2. - (1 + e)/e*(pow(1 + e, N_border/2 - i) - 1);
	else if (i > N_border/2 + N_central - 1) return (N_central - 1)/2. + (1 + e)/e*(pow(1 + e, i - (N_border/2 + N_central - 1)) - 1);
	else return i - (N_central + N_border - 1)/2.;
}

void InitPoleGauss(complex<double> *arr, 
		int n_x, int n_y, int part_x, int part_y, 
		int NX_central, int NX_border, double dx, double ex,
		int NY_central, int NY_border, double dy, double ey)
// n_x - number of local columns, n_y - number of local rows
// part_x, part_y - decart coordinates of part
// N_central - whole central part, N_border - both sides
// dx - minimal reproducible area, e - step increment
{
	int i, j;
	double r2;

	for (i = 0; i < n_y; i++) for (j = 0; j < n_x; j++)
	{
		r2 = sqr(coord_dimensionless(i + part_y*n_y, NY_central, NY_border, ey))*sqr(dx) + sqr(coord_dimensionless(j + part_x*n_x, NX_central, NX_border, ex))*sqr(dy);
		r2 /= sqr(rad);
		arr[i*n_x + j] = std::complex<double>(exp(-r2/2.), 0.);
	}
}

void InitPoleFile(complex<double> *arr, int local_length, int part_x, int part_y, char *file_name)
{
	MPI_File p_f;
	MPI_File_open(comm_cart, file_name, MPI_MODE_RDONLY, MPI_INFO_NULL, &p_f);
	MPI_File_set_view(p_f, 0, MPI_DOUBLE_COMPLEX, filetype_darray_complex, "native", MPI_INFO_NULL);
	MPI_File_read_all(p_f, arr, local_length, MPI_DOUBLE_COMPLEX, MPI_STATUS_IGNORE);
	MPI_File_close(&p_f);
}

void Allocate()
{
	int k;

	dx = new double[local_sizes[1] + 1];
	dy = new double[local_sizes[0] + 1];

	if (rank_row == 0)
		for (k = 1; k < local_sizes[1] + 1; k++) dx[k] = step(rank_row*local_sizes[1] + k - 1, gsizes1[1], gsizes2[1], eps[1], min_dxdy[1]/rad);
	else if (rank_row == psizes[1] - 1)
		for (k = 0; k < local_sizes[1]; k++) dx[k] = step(rank_row*local_sizes[1] + k - 1, gsizes1[1], gsizes2[1], eps[1], min_dxdy[1]/rad);
	else
		for (k = 0; k < local_sizes[1] + 1; k++) dx[k] = step(rank_row*local_sizes[1] + k - 1, gsizes1[1], gsizes2[1], eps[1], min_dxdy[1]/rad);

	if (rank_col == 0)
		for (k = 1; k < local_sizes[0] + 1; k++) dy[k] = step(rank_col*local_sizes[0] + k - 1, gsizes1[0], gsizes2[0], eps[0], min_dxdy[0]/rad);
	else if (rank_col == psizes[0] - 1)
		for (k = 0; k < local_sizes[0]; k++) dy[k] = step(rank_col*local_sizes[0] + k - 1, gsizes1[0], gsizes2[0], eps[0], min_dxdy[0]/rad);
	else
		for (k = 0; k < local_sizes[0] + 1; k++) dy[k] = step(rank_col*local_sizes[0] + k - 1, gsizes1[0], gsizes2[0], eps[0], min_dxdy[0]/rad);

	sweep_a_x = new complex<double>[local_sizes[1]];
	sweep_b_x = new complex<double>[local_sizes[1]];
	sweep_c_x = new complex<double>[local_sizes[1]];
	sweep_f_x = new complex<double>[local_sizes[1]];
	sweep_a_y = new complex<double>[local_sizes[0]];
	sweep_b_y = new complex<double>[local_sizes[0]];
	sweep_c_y = new complex<double>[local_sizes[0]];
	sweep_f_y = new complex<double>[local_sizes[0]];
	buf_x = new complex<double>[local_sizes[0]*local_sizes[1]];
	buf_y = new complex<double>[local_sizes[0]*local_sizes[1]];
	edge_l = new complex<double>[local_sizes[0]];
	edge_r = new complex<double>[local_sizes[0]];
	edge_t = new complex<double>[local_sizes[1]];
	edge_b = new complex<double>[local_sizes[1]];
	send_x = new complex<double>[local_sizes[0]];
	send_y = new complex<double>[local_sizes[1]];
	if (rank_row == 0)
		for (k = 1; k < local_sizes[1]; k++)
		{
			sweep_a_x[k] = 0.5/dx[k];
			sweep_b_x[k] = 0.5/dx[k + 1];
		}
	else if (rank_row == psizes[1] - 1)
		for (k = 0; k < local_sizes[1] - 1; k++)
		{
			sweep_a_x[k] = 0.5/dx[k];
			sweep_b_x[k] = 0.5/dx[k + 1];
		}
	else
		for (k = 0; k < local_sizes[1]; k++)
		{
			sweep_a_x[k] = 0.5/dx[k];
			sweep_b_x[k] = 0.5/dx[k + 1];
		}

	if (rank_col == 0)
		for (k = 1; k < local_sizes[0]; k++)
		{
			sweep_a_y[k] = 0.5/dy[k];
			sweep_b_y[k] = 0.5/dy[k + 1];
		}
	else if (rank_col == psizes[0] - 1)
		for (k = 0; k < local_sizes[0] - 1; k++)
		{
			sweep_a_y[k] = 0.5/dy[k];
			sweep_b_y[k] = 0.5/dy[k + 1];
		}
	else
		for (k = 0; k < local_sizes[0]; k++)
		{
			sweep_a_y[k] = 0.5/dy[k];
			sweep_b_y[k] = 0.5/dy[k + 1];
		}

	EEE = new complex<double>[local_sizes[0]*local_sizes[1]];

	MPI_Barrier(MPI_COMM_WORLD);
}
