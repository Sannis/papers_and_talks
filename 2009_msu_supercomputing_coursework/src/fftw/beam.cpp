#include <cstdio>
#include <complex>
#include <cmath>
#include <cstdlib>
using namespace std;

#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>

#include <fftw_mpi.h>

#include "get_options.cpp"
#include "grid_settings.cpp"
#include "data_output.cpp"
#include "beam_settings.cpp"
#include "allocate.cpp"
#include "propagation.cpp"
#include "finalize.cpp"

int main(int argc, char **argv)
{
	// Consts
	const double dphi_tres = 0.01;
	
	// Default parameters
	strcpy(results_root_path, "./results");
	fftw_no_buffer_flag = 0; // Use buffer
	fftw_transpose_flag = 0; // Don't do transpose every step
	fftw_measure_flag = 0; // Don't do measure on plan creation
	R = 0; // Linear propagation
	
	// Initialize MPI
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	// Parse program arguments
	if( rank == 0 ) {
		get_program_options(argc, argv);
		sprintf(results_path, "%s/N%d_NP%d=%dx%d_R%lf", results_root_path, N, size, 1, size, R);
		
		char mkdir_cmd[filename_length_max];
		
		sprintf(mkdir_cmd, "mkdir -p %s", results_path);
		system(mkdir_cmd);
		sprintf(mkdir_cmd, "rm -f %s/*", results_path);
		system(mkdir_cmd);
		
		// Write log_content
		sprintf(log_content                      , "    =====================================================\n");
		sprintf(&log_content[strlen(log_content)], "    ===       Propagation: Diffraction and Kerr       ===\n");
		sprintf(&log_content[strlen(log_content)], "    === FFT(using FFTW) + simple nonlinearity method  ===\n");
		sprintf(&log_content[strlen(log_content)], "    =====================================================\n\n");
		
		sprintf(&log_content[strlen(log_content)], "MPI grid size: %d (%dx%d)\n", size, 1, size);
		
		sprintf(&log_content[strlen(log_content)], "\nImpulse parameters:\n");
		sprintf(&log_content[strlen(log_content)], "    N: %d\n", N);
		sprintf(&log_content[strlen(log_content)], "    L: %lf\n", Lx);
		
		sprintf(&log_content[strlen(log_content)], "\nCalculation parameters:\n");
		sprintf(&log_content[strlen(log_content)], "    z_final: %lf\n", z_final);
		sprintf(&log_content[strlen(log_content)], "    dz: %lf\n", dz);
		sprintf(&log_content[strlen(log_content)], "    R: %lf\n", R);
		sprintf(&log_content[strlen(log_content)], "    dphi=0.5*R*dz: %lf ", 0.5*R*dz);
		if ( 0.5*R*dz < dphi_tres ) {
			sprintf(&log_content[strlen(log_content)], " < %lf => good :)\n", dphi_tres);
		} else {
			sprintf(&log_content[strlen(log_content)], " > %lf => bad [!]\n", dphi_tres);
		}
		
		sprintf(&log_content[strlen(log_content)], "\nFFTW options:\n");
		if ( fftw_no_buffer_flag ) {
			sprintf(&log_content[strlen(log_content)], "    Buffer: no\n");
		} else {
			sprintf(&log_content[strlen(log_content)], "    Buffer: yes\n");
		}
		
		if ( fftw_transpose_flag ) {
			sprintf(&log_content[strlen(log_content)], "    Do transpose: yes\n");
		} else {
			sprintf(&log_content[strlen(log_content)], "    Do transpose: no\n");
		}
		
		if ( fftw_measure_flag ) {
			sprintf(&log_content[strlen(log_content)], "    Do measure: yes\n");
		} else {
			sprintf(&log_content[strlen(log_content)], "    Do measure: no\n");
		}
		
		sprintf(&log_content[strlen(log_content)], "\nn        dz          z           I_max_exp(z)      I_max_theor(z)    P(z)\n");
		
		printf("%s", log_content);
		
		// Generate log_content file name
		sprintf(filename_log, "%s/log.txt", results_path);
		
		Write_Log(filename_log, log_content);
	}
	
	// Broadcast parameters from root to all processes
	MPI_Bcast(&N, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	MPI_Bcast(&Lx, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	MPI_Bcast(&steps_between_saves, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	
	MPI_Bcast(&fftw_no_buffer_flag, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	MPI_Bcast(&fftw_transpose_flag, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
	
	MPI_Bcast(&z_final, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&dz, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	MPI_Bcast(&R, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
	
	//MPI_Bcast(filename_input, filename_max_length, MPI_CHAR, 0, MPI_COMM_WORLD);
	MPI_Bcast(results_path, filename_length_max, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	// Calculate other variables after parameters request
	dx = dy = Lx/(N-1);
	
	coordinats();
	
	create_plans();
	
	double start, finish, time_local, time_global;
	
	start = -MPI_Wtime();
	
	Propagation();
	
	finish = MPI_Wtime();
	time_local = start + finish;
	
	MPI_Reduce(&time_local, &time_global, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	
	if ( rank == 0 ) {
		sprintf(log_content, "\nExecution time(sec):\n%lf\n", time_global);
		printf("%s", log_content);
		Write_Log(filename_log, log_content);
	}

	Finalize();
	return(0);
}

