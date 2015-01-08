extern complex<double> *local_data;
extern double dz, dx, dy;
extern int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size;
extern int size,rank;
extern int N;
extern double P_glob;
extern double I_max;

void Integral_Power()
	{
		double P_loc = 0 ;
		P_glob = 0 ;
		for (int i= 0; i < local_nx; ++i)
 			{
    				 for (int j = 0; j < N; ++j)
	 				{
		 				P_loc = P_loc + norm(local_data[i*N  + j] );
	 				}
			 }
		P_loc *= dx*dy/M_PI; 
		MPI_Allreduce(&P_loc, &P_glob, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);	
	}
	
void I_maximum()
	{
		I_max = 0;
		double I_max_loc = 0;
		for (int i= 0; i < local_nx; ++i)
 			{
    				 for (int j = 0; j < N; ++j)
	 				{
		 				if ( norm( local_data[i*N  + j] ) > I_max_loc ) I_max_loc =  norm(local_data[i*N  + j]) ;
	 				}
			 }
		MPI_Allreduce(&I_max_loc, &I_max, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
	}
