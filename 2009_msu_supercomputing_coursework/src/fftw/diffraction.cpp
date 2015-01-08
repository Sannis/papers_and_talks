
extern int N;
extern double *x;
extern double *y;
extern double *output;
extern double Lx, dx,dy,dz;
extern complex<double> *local_data;
extern complex<double> *local_temp;
extern int n_fields;

extern int local_nx;
extern int local_x_start;
extern int local_ny_after_transpose;
extern int local_y_start_after_transpose;
extern int total_local_size;

extern int size,rank;

int fftw_no_buffer_flag;
int fftw_transpose_flag;
fftwnd_mpi_output_order fftw_transpose_order;

void Diffraction()
{
		int i = 0, j = 0;
		int ki = 0, kj = 0;
		complex<double> DifrKoeff(0, 2*M_PI*M_PI/(N - 1)/(N - 1)/dx/dx*dz);
		
		// Forward FFT
		fftwnd_mpi(p, n_fields, (fftw_complex*)local_data, (fftw_complex*)local_temp, fftw_transpose_order);
		
		// Calculate diffraction transform of the spectrum
        for (i = 0; i < local_ny_after_transpose; ++i)
        {
            for (j = 0; j < N; ++j)
            {
                if ((i + local_y_start_after_transpose)< N/2)
                {
                    ki = (i + local_y_start_after_transpose )*(i + local_y_start_after_transpose );
                }
                else
                {
                    ki = (i + local_y_start_after_transpose - N )*(i + local_y_start_after_transpose  - N);
                }

                if (j< N/2)
                {
                    kj = j*j;
                }
                else
                {
                    kj = (j - N )*(j - N );
                }

                local_data[i*N  + j] = local_data[i*N  + j] * exp(DifrKoeff * (double)(ki + kj));
            }
        }

        // Backward FFT
		fftwnd_mpi(p2, n_fields, (fftw_complex*)local_data, (fftw_complex*)local_temp, fftw_transpose_order);
		
		// Fix values, this is a "feature" of FFT
		for (i = 0; i < local_nx2; ++i)
 		{
     		for (j = 0; j < N; ++j)
	 		{
		 		local_data[i*N + j] = local_data[i*N + j]/(double)N/(double)N;
	 		}
 		}
}

