
extern int N;
extern double *x;
extern double *y;
extern double Lx, dx,dy;
extern complex<double> *local_data;
extern int n_fields ;
extern int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size;

void coordinats()
{
	x = new double[N];
	y = new double[N];
	for (int i = 0 ; i < N ; ++i ) 
		{
			x[i] = -Lx/2 + dx*i;
			y[i] = -Lx/2 + dy*i;
		}
}	

void input_beam()
{
	for (int i = 0; i < local_nx; ++i)
 			{
    				 for (int j = 0; j < N; ++j)
	 				{
		 				local_data[i*N  + j] = exp(-(x[i + local_x_start]*x[i + local_x_start]+y[j]*y[j])/2.0);
	 				}
			 }
}
