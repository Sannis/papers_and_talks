extern MPI_File fh;
extern MPI_Status status;
extern int N;
extern double *x;
extern double *y;
extern double Lx, dx,dy;
extern int Nz;
extern double z_final;
extern complex<double> *local_data;
extern complex<double> *local_temp;
extern double P_glob ;
extern double I_max;
extern int n_fields ;
extern int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size;
extern int size,rank;

int steps_between_saves = 10;

#include "diffraction.cpp"
#include "nonlinearity.cpp"
#include "integrals.cpp"

void Propagation()
{
	local_data = new complex<double>[total_local_size];
	
	if( fftw_no_buffer_flag ) {
		local_temp = NULL;
	} else {
		local_temp = new complex<double>[total_local_size];
	}
	
	fftw_transpose_order = fftw_transpose_flag ? FFTW_NORMAL_ORDER : FFTW_TRANSPOSED_ORDER; // This isn't a bug!
	
	input_beam();
	
	// Propagation loop
	int iz = 0;
	double z = 0;
	for( iz = 0; iz < Nz, z < z_final+dz/2; ++iz )
	{
		if( steps_between_saves > 0 && (iz % steps_between_saves) == 0 ) {
			Data_Output(iz);
			
			Integral_Power();
			I_maximum();
			
			if (rank == 0) {
				sprintf(log_content, "%05d    %4.6lf    %4.6lf    %4.12lf    %4.12lf    %4.12lf\n",
									  iz,     dz,       z,        I_max,     1/(1 + z*z), P_glob);
				printf("%s", log_content);
				Write_Log(filename_log, log_content);
			}
		}
		
		Diffraction();
		Kerr_Nonlinear();
		
		z += dz;
	}
	
	Data_Output(Nz); // Parallel output
}
