extern int N;

// координатная сетка
double *x;
double *y;

// Integrals
double P_glob ;
double I_max;

// параллельный вывод

const int filename_length_max = 1024;
char filename_binary[filename_length_max];
char filename_log[filename_length_max];
char results_root_path[filename_length_max];
char results_path[filename_length_max];

MPI_File fh;

const int log_size_big = 2048;
char log_content[log_size_big];

// параллельное Фурье
int size,rank;
MPI_Status status;
complex<double> *local_data;
complex<double> *local_temp;
double *output;
int n_fields = 1;
int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size, 
	local_nx2,
	local_x_start2,
	local_ny_after_transpose2,
	local_y_start_after_transpose2,
	total_local_size2;
// plan FFT
fftwnd_mpi_plan p, p2;

int fftw_measure_flag;

void create_plans()
{
	if(fftw_measure_flag) {
		p = fftw2d_mpi_create_plan(MPI_COMM_WORLD,N, N,FFTW_FORWARD, FFTW_MEASURE);
	} else {
		p = fftw2d_mpi_create_plan(MPI_COMM_WORLD,N, N,FFTW_FORWARD, FFTW_ESTIMATE);
	}
	
	fftwnd_mpi_local_sizes(	p,
				&local_nx,
				&local_x_start,
				&local_ny_after_transpose,
				&local_y_start_after_transpose,
				&total_local_size
	);
	
	if(fftw_measure_flag) {
		p2 = fftw2d_mpi_create_plan(MPI_COMM_WORLD,N, N,FFTW_BACKWARD, FFTW_MEASURE);
	} else {
		p2 = fftw2d_mpi_create_plan(MPI_COMM_WORLD,N, N,FFTW_BACKWARD, FFTW_ESTIMATE);
	}
	
	fftwnd_mpi_local_sizes(	p2,
				&local_nx2,
				&local_x_start2,
				&local_ny_after_transpose2,
				&local_y_start_after_transpose2,
				&total_local_size2
	);
}

