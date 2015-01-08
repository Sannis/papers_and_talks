#include "calculation.cpp"

//==========//============//==========//==========//==========//

extern complex<double> _COMPLEX_I;

extern char dir_name[1024];
extern char init_file_name[1024];
extern char mode;

extern int gsizes1[2], gsizes2[2], gsizes[2];
extern double al, eps[2], rad, min_dxdy[2];

extern long int schmax;
extern double dz, cur_z;
extern double PHASE_THRESHOLD_HIGH, PHASE_THRESHOLD_LOW;

extern double ldiff, kerrR;

extern complex<double> *EEE;

extern MPI_Comm comm_cart;
extern int rank, size;

extern char log_name[1024];
extern FILE *p_log;

double global_max_pole, global_max_phase;
long int sch;
char out_name[1024];

//==========//============//==========//==========//==========//

void Tact()
{
	double max_pole = 0.;

	cur_z += dz;
	
	Diffraction(EEE, dz);
	Kerr(EEE, local_sizes[1], local_sizes[0], dz, &max_pole);
	
	MPI_Allreduce(&max_pole, &global_max_pole, 1, MPI_DOUBLE, MPI_MAX, comm_cart);	
	global_max_phase = global_max_pole*kerrR/2.*dz;
	if (global_max_phase > 0) {
		if ((global_max_phase > PHASE_THRESHOLD_HIGH) || (global_max_phase < PHASE_THRESHOLD_LOW)) dz *= PHASE_THRESHOLD_LOW/global_max_phase;
    }
}

void Save()
{
	if ((mode == 's') && (sch % 10 == 0))
	{
		sprintf(out_name, "%s/out_%.5ld.cpl", dir_name, sch);
		OutComplexBinaryParallel(comm_cart, &(EEE[0]), local_sizes[0]*local_sizes[1], out_name);
	}

	double loc_int = 0., glob_int;
	for (int k = 0; k < local_sizes[0]*local_sizes[1]; k++) loc_int += norm(EEE[k]);
	MPI_Reduce(&(loc_int), &(glob_int), 1, MPI_DOUBLE, MPI_SUM, 0, comm_cart);
	if (rank == 0)
	{
		p_log = fopen(log_name, "a+t");
		fprintf(p_log, "%.5ld   %.6f   %.6f   %.12f   %.12f\n", sch, dz, cur_z, global_max_pole, glob_int*min_dxdy[0]*min_dxdy[1]/M_PI/sqr(rad));
		fclose(p_log);
	}
}

void Propagate()
{
	sch = 0;
	cur_z = 0.;
	InitPoleGauss(EEE, local_sizes[1], local_sizes[0], rank_row, rank_col, gsizes1[1], gsizes2[1], min_dxdy[1], eps[1], gsizes1[0], gsizes2[0], min_dxdy[0], eps[0]);
	//InitPoleFile(EEE, local_sizes[1]*local_sizes[0], rank_row, rank_col, init_file_name);
	MPI_Barrier(comm_cart);
	Save();
	
	double t_start = MPI_Wtime();
	
	while (sch < schmax)
	{
		sch++;
		Tact();
		Save();

		if (cur_z > 1.) break;
	}
	
	double t_finish = MPI_Wtime();
	double loc_time = t_finish - t_start;
	double glob_time;
	MPI_Reduce(&loc_time, &glob_time, 1, MPI_DOUBLE, MPI_MAX, 0, comm_cart);
	if (rank == 0)
	{
		p_log = fopen(log_name, "a+t");
		fprintf(p_log, "\nExecution time (sec):\n%.6f\n", glob_time);
		fclose(p_log);
	}

	sprintf(out_name, "%s/out_z1.cpl", dir_name);
	OutComplexBinaryParallel(comm_cart, &(EEE[0]), local_sizes[0]*local_sizes[1], out_name);
}
