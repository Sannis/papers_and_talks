extern complex<double> *local_data;
extern double dz;
extern int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size;
extern int size,rank;
extern int N;
double PHASE_THRESHOLD_HIGH = 0.1;							// threshold for step decrement
double PHASE_THRESHOLD_LOW = 0.8*PHASE_THRESHOLD_HIGH;					// threshold for step increment

double R = 0;

void Kerr_Nonlinear()
	{
		double fi = 0 , max_fi = 0 ,global_max_phase;
		complex<double> Kerr_Coeff(0,R*dz/2.) ;
		
		for (int i= 0; i < local_nx; ++i)
 			{
    				 for (int j = 0; j < N; ++j)
	 				{
		 				local_data[i*N  + j] *= exp( - Kerr_Coeff * norm(local_data[i*N  + j]) );
						fi = R*dz/2. *norm(local_data[i*N  + j]);
						if (fi > max_fi) max_fi = fi;
	 				}
			 }
		
		MPI_Allreduce(&max_fi, &global_max_phase, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);	
		//printf(" glob max fi %lf %d\n", global_max_phase,rank);
		if (global_max_phase != 0)
			{
				if ((global_max_phase > PHASE_THRESHOLD_HIGH) || (global_max_phase < PHASE_THRESHOLD_LOW)) dz *= PHASE_THRESHOLD_LOW/global_max_phase;
				//printf(" dz after %lf %d\n", dz,rank);
			}

	}

