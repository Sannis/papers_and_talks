// Library includes
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <complex>
using namespace std;

/// TODO: Implement this
//#include <omp.h>

#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#endif
#include <getopt.h>


// Project includes
#include "param_struct.h"
#include "sns_mpi.h"
#include "sns_utility.h"

#include "runge_kutta_4.h"

// Global defines
#define VERSION "0.2"
#define AUTHOR "Oleg Efimov"
#define BUGREPORT_EMAIL "efimovov@gmail.com"

//---------------------------------------------------------------------------
const char* get_program_name(const char *argv0)
{
	char* ch = strrchr(argv0, '/');
	if ( ch != NULL ) {
		return ch + 1;
	} else {
	    return argv0;
	}
}
//---------------------------------------------------------------------------
void display_help(const char *argv0)
{
	const char *program_name = get_program_name(argv0);
	
	printf("Usage: %s [OPTION...]\n", program_name);
	printf("MPI program for impulse propagation research considering diffraction and Kerr nonlinearity.\n");
	printf("Runge-Kutta 4th order method.\n");

	printf("\nOptions:\n");
	printf("  -n    number of grid points in cross-section\n");
	printf("  -l    half-width of impulse\n\n");

	printf("  --impulse-file=<filename>    file with initial conditions, complex impulse shape\n");
	printf("  --results-root=<path>        path for results directories\n\n");

	printf("  -r    Kerr koefficient\n");
	printf("  -z    step by z-axis\n");
	printf("  -s    number of steps\n");
	printf("  -d    number of steps between saving data\n\n");
	
	printf(" --npx, --npy number of processes in array distribution by X and Y axis\n\n");
	
	printf("  --debug    displays debug information and save data to ./results/debug\n");
	printf("  -h    displays this stupid help information\n");
	printf("  -v    displays program version\n");
	
	printf("\nExamples:\n");
	printf("user@skif> mpirun -np 16 %s -n 512 --npx=4 --npy=4\n", program_name);
	printf("user@fen1> mpisubmit.bg -n 32 %s -- -n 512 --npx=8 --npy=4\n", program_name);
	
	printf("\nAuthor: %s.\n", AUTHOR);
	printf("Report bugs to <%s>.\n", BUGREPORT_EMAIL);
}
//---------------------------------------------------------------------------
void display_version(const char *argv0)
{
	printf("%s version %s\n", get_program_name(argv0), VERSION);
	printf("MPI program for impulse propagation research considering diffraction and Kerr nonlinearity.\n");
	printf("Runge-Kutta 4th order method.\n");
	
	printf("\nAuthor: %s.\n", AUTHOR);
	printf("Report bugs to <%s>.\n", BUGREPORT_EMAIL);
}
//---------------------------------------------------------------------------
void get_program_options(int argc, char *argv[], param_struct *p_parameters)
{
	int c = 0;
	opterr = 0;

	static struct option long_options[] = {
		{"impulse-file", required_argument, NULL, 0},
		{"results-root", required_argument, NULL, 0},
		{"z-final", required_argument, NULL, 0},
		{"npx", required_argument, NULL, 0},
		{"npy", required_argument, NULL, 0},
		{"debug", no_argument, NULL, 0},
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'}
	};
	int option_index = 0;
	
	while ( (c = getopt_long(argc, argv, "n:l:r:z:d:hv", long_options, &option_index)) != -1 ) {
		switch (c) {
			case 0:
				if( strcmp(long_options[option_index].name, "impulse-file") == 0 ) {
					strcpy(p_parameters->filename_input, optarg);
				} else if( strcmp(long_options[option_index].name, "results-root") == 0 ) {
					strcpy(p_parameters->results_root_path, optarg);
				} else if( strcmp(long_options[option_index].name, "z-final") == 0 ) {
					p_parameters->z_final = atof(optarg);
				} else if( strcmp(long_options[option_index].name, "npx") == 0 ) {
					sscanf(optarg, "%d", &p_parameters->npx);
				} else if( strcmp(long_options[option_index].name, "npy") == 0 ) {
					sscanf(optarg, "%d", &p_parameters->npy);
				} else if( strcmp(long_options[option_index].name, "debug") == 0 ) {
					p_parameters->debug = 1;
				}
				break;

			case 'n':
				p_parameters->n = atoi(optarg);
				break;
			case 'l':
				p_parameters->l = atof(optarg);
				break;
			case 'r':
				p_parameters->R = atof(optarg);
				break;
			case 'z':
				p_parameters->dz = atof(optarg);
				break;
			case 'd':
				p_parameters->steps_between_saves = atoi(optarg);
				break;

			case 'h':
				display_help(argv[0]);
				exit(0);
			case 'v':
				display_version(argv[0]);
				exit(0);
			default:
				break;
		}
	}
	
	if( p_parameters->filename_input[0] == '\0' ) {
	    printf("ERROR: You must specify impulse filename.\n");
	    display_help(argv[0]);
	    exit(0);
	}
	
	if( p_parameters->results_root_path[0] == '\0' ) {
	    printf("ERROR: You must specify root path for results.\n");
	    display_help(argv[0]);
	    exit(0);
	}
}

//---------------------------------------------------------------------------
int main(int argc, char *argv[])
{
	// Variables
	param_struct p_parameters;
	sns::mpi::struct_mpi_data s_mpi_data;
	complex<double> *local_field;
	complex<double> **temp_arrays;
	double intensity_maximum, intensity_integral;
	
	const int log_size_big = 2048;
	const int log_size_small = 256;
	char *log;
	
	int filename_max_length = 512;
	int dir_max_length = 128;
	char *results_path;
	char *filename_out;
	char *filename_log;
	char *results_subfolder;
	
	double start, finish, time_local, time_global;
	
	int i, j;
	
	// Default parameters
	p_parameters.n = 128;
	p_parameters.l = 10;
	
	p_parameters.steps_between_saves = 1;
	
	p_parameters.z_final = 1.0;
	p_parameters.dz = 0.01;
	p_parameters.R = 0;
	
	p_parameters.npx = 0;
	p_parameters.npy = 0;
	p_parameters.filename_input = new char[filename_max_length];
	p_parameters.filename_input[0] = 0;
	p_parameters.results_root_path = new char[filename_max_length];
	p_parameters.filename_input[0] = 0;
	
	p_parameters.debug = 0;
	
	// Initialize MPI, get process rank
	MPI_Init(&argc, &argv);
	s_mpi_data.mpi_comm = MPI_COMM_WORLD;
	MPI_Comm_rank(s_mpi_data.mpi_comm, &s_mpi_data.mpi_rank);
	MPI_Comm_size(s_mpi_data.mpi_comm, &s_mpi_data.mpi_size);
	
 	filename_out = new char[filename_max_length];
	if( !filename_out ) {
		MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
		return 1;
	}
	
	// Parse program command line options
	if( s_mpi_data.mpi_rank == 0 ) {
		get_program_options(argc, argv, &p_parameters);
		
		// Check input parameters
		if( s_mpi_data.mpi_size != p_parameters.npx*p_parameters.npy ){
			printf("ERROR: Wrong processes number!\n");
			printf("%d != %dx%d.\n\n", s_mpi_data.mpi_size, p_parameters.npx, p_parameters.npy);
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
		
		// Set MPI-specific parameters to s_mpi_data struct
		s_mpi_data.n = p_parameters.n;
		s_mpi_data.npx = p_parameters.npx;
		s_mpi_data.npy = p_parameters.npy;
		
		// Generate path for result files
		results_subfolder = new char[dir_max_length];
				
		if(p_parameters.debug) {
			sprintf(results_subfolder, "%s", "debug");
		} else {
			sprintf(results_subfolder, "N%d_NP%d=%dx%d_R%lf", s_mpi_data.n, s_mpi_data.mpi_size, s_mpi_data.npx, s_mpi_data.npy, p_parameters.R);
		}
		
		results_path = sns::utility::get_results_path(filename_max_length, p_parameters.results_root_path, results_subfolder);
		
		if( !results_path ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
		
		// Allocate memory for parameters log
		log = new char[log_size_big];
		if( !log ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
		
		// Write log
		sprintf(log              , "    =========================================\n");
		sprintf(log + strlen(log), "    === Propagation: Diffraction and Kerr ===\n");
		sprintf(log + strlen(log), "    ===   Runge-Kutta 4th order method    ===\n");
		sprintf(log + strlen(log), "    =========================================\n\n");
		
		sprintf(log + strlen(log), "MPI grid size: %d (%dx%d)\n", s_mpi_data.mpi_size, s_mpi_data.npx, s_mpi_data.npy);
		
		sprintf(log + strlen(log), "\nImpulse parameters:\n");
		sprintf(log + strlen(log), "    N: %d\n", p_parameters.n);
		sprintf(log + strlen(log), "    L: %lf\n", p_parameters.l);
		if( p_parameters.filename_input[0] != 0 ) {
			sprintf(log + strlen(log), "    Impulse file: %s\n", p_parameters.filename_input);
		} else {
			sprintf(log + strlen(log), "    Impulse file: (none)\n");
		}
		
		sprintf(log + strlen(log), "\nCalculation parameters:\n");
		sprintf(log + strlen(log), "    z_final: %lf\n", p_parameters.z_final);
		sprintf(log + strlen(log), "    dz: %lf\n", p_parameters.dz);
		sprintf(log + strlen(log), "    R: %lf\n", p_parameters.R);
		sprintf(log + strlen(log), "    dphi=0.5*R*dz: %lf, ", 0.5*p_parameters.R*p_parameters.dz);
		if ( 0.5*p_parameters.R*p_parameters.dz < 0.01 ) {
			sprintf(log + strlen(log), "good :)\n");
		} else {
			sprintf(log + strlen(log), "bad [!]\n");
		}
		
		sprintf(log + strlen(log), "n        dz          z           I_max_exp(z)      I_max_theor(z)    P(z)\n");
		
		printf("%s", log);
		
		// Generate log file name
		filename_log = new char[filename_max_length];
		
		if( !filename_log ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
		
		sprintf(filename_log, "%s/log.txt", results_path);
		
		sns::utility::write_log(filename_log, log);
		
		delete[] log;
	} else {
		results_path = new char[filename_max_length];
		
		if( !results_path ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
	}
	
	// Broadcast parameters from root to all processes
	MPI_Bcast(&s_mpi_data.n, 1, MPI_INTEGER, 0, s_mpi_data.mpi_comm);
	MPI_Bcast(&p_parameters.l, 1, MPI_DOUBLE, 0, s_mpi_data.mpi_comm);
	
	MPI_Bcast(&s_mpi_data.npx, 1, MPI_INTEGER, 0, s_mpi_data.mpi_comm);
	MPI_Bcast(&s_mpi_data.npy, 1, MPI_INTEGER, 0, s_mpi_data.mpi_comm);
	
	MPI_Bcast(&p_parameters.steps_between_saves, 1, MPI_INTEGER, 0, s_mpi_data.mpi_comm);
	
	MPI_Bcast(&p_parameters.dz, 1, MPI_DOUBLE, 0, s_mpi_data.mpi_comm);
	MPI_Bcast(&p_parameters.z_final, 1, MPI_DOUBLE, 0, s_mpi_data.mpi_comm);
	MPI_Bcast(&p_parameters.R, 1, MPI_DOUBLE, 0, s_mpi_data.mpi_comm);
	
	MPI_Bcast(&p_parameters.debug, 1, MPI_INTEGER, 0, s_mpi_data.mpi_comm);
	
	MPI_Bcast(p_parameters.filename_input, filename_max_length, MPI_CHAR, 0, s_mpi_data.mpi_comm);
	MPI_Bcast(results_path, filename_max_length, MPI_CHAR, 0, s_mpi_data.mpi_comm);
	
	// Calculate local sizes
	s_mpi_data.local_nx = s_mpi_data.n/s_mpi_data.npx;
	s_mpi_data.local_ny = s_mpi_data.n/s_mpi_data.npy;
	s_mpi_data.total_local_size = s_mpi_data.local_nx*s_mpi_data.local_ny;
	
	// Allocate memory for local part of impulse
	local_field = new complex<double>[s_mpi_data.total_local_size];
	if( !local_field ) {
		MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
		return MPI_ERR_OTHER;
	}
	
	// Allocate memory for local parts of temporary arrays
	temp_arrays = new complex<double>*[rk4::temp_arrays_count_required];
	
	if( !temp_arrays ) {
		MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
		return MPI_ERR_OTHER;
	}
	
	for( i = 0; i < rk4::temp_arrays_count_required; i++ ) {
		temp_arrays[i] = new complex<double>[(s_mpi_data.local_nx + 2)*(s_mpi_data.local_ny + 2)];
		
		if( !temp_arrays[i] ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
		
		for( j = 0; j < (s_mpi_data.local_nx + 2)*(s_mpi_data.local_ny + 2); j++ ) {
			temp_arrays[i][j] = 0;
		}
	}
	
	// Load initial impulse
	sns::mpi::load_darray(local_field, &s_mpi_data, MPI_DOUBLE_COMPLEX, p_parameters.filename_input);
	
	// Allocate memory for log string
	if ( s_mpi_data.mpi_rank == 0 ) {
		log = new char[log_size_small];
		
		if( !log ) {
			MPI_Abort(s_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return MPI_ERR_OTHER;
		}
	}
	
	// Save initial impulse to results directory
	sprintf(filename_out, "%s/out00000.cpl", results_path);
	sns::mpi::save_darray(local_field, &s_mpi_data, MPI_DOUBLE_COMPLEX, filename_out);

	intensity_maximum = sns::mpi::calc_intensity_maximum(local_field, &s_mpi_data);
	intensity_integral = sns::mpi::calc_intensity_integral(local_field, &s_mpi_data);
	intensity_integral *= 4*p_parameters.l*p_parameters.l/s_mpi_data.n/s_mpi_data.n/M_PI;
	if ( s_mpi_data.mpi_rank == 0 ) {
		sprintf(log, "%05d    %4.6lf    %4.6lf    %4.12lf    %4.12lf    %4.12lf\n",
		              0, p_parameters.dz, 0.0, intensity_maximum, 1.0, intensity_integral);
		printf("%s", log);
		sns::utility::write_log(filename_log, log);
	}
	
	int s = 0; // Counter for steps
	double z_current = 0; // Current z coordinate
	
	// Start timer
	start = MPI_Wtime();
	
	// Do calculations steps
	do {
		// Step
		rk4::do_step(local_field, &s_mpi_data, &p_parameters, temp_arrays);
		
		s++;
		z_current += p_parameters.dz;
		
		// Save results on some steps
		if ( p_parameters.steps_between_saves > 0 && (s % p_parameters.steps_between_saves) == 0 ) {
			sprintf(filename_out, "%s/out%05d.cpl", results_path, s);
			sns::mpi::save_darray(local_field, &s_mpi_data, MPI_DOUBLE_COMPLEX, filename_out);
			
			intensity_maximum = sns::mpi::calc_intensity_maximum(local_field, &s_mpi_data);
			intensity_integral = sns::mpi::calc_intensity_integral(local_field, &s_mpi_data);
			intensity_integral *= 4*p_parameters.l*p_parameters.l/s_mpi_data.n/s_mpi_data.n/M_PI;
			if ( s_mpi_data.mpi_rank == 0 ) {
				sprintf(log, "%05d    %4.6lf    %4.6lf    %4.12lf    %4.12lf    %4.12lf\n",
				              s, p_parameters.dz, z_current, intensity_maximum, 1/(1 + z_current*z_current), intensity_integral);
				printf("%s", log);
				sns::utility::write_log(filename_log, log);
			}
		}
	} while ( z_current < p_parameters.z_final );
	
	// Stop timer
	finish = MPI_Wtime();
	
	time_local = finish - start;
	
	MPI_Reduce(&time_local, &time_global, 1, MPI_DOUBLE, MPI_MAX, 0, s_mpi_data.mpi_comm);
	
	if ( s_mpi_data.mpi_rank == 0 ) {
		sprintf(log, "\nExecution time(sec):\n%lf\n", time_global);
		printf("%s", log);
		sns::utility::write_log(filename_log, log);
	}
	
	// Save final impulse to results directory
	sprintf(filename_out, "%s/out_z%4.6lf.cpl", results_path, z_current);
	sns::mpi::save_darray(local_field, &s_mpi_data, MPI_DOUBLE_COMPLEX, filename_out);
	
	// Free memory
	if ( s_mpi_data.mpi_rank == 0 ) {
		delete[] filename_log;
		delete[] log;
	}
	
	delete[] local_field;
	delete[] filename_out;
	delete[] p_parameters.filename_input;
	
	for( i = 0; i < rk4::temp_arrays_count_required; i++ ) {
		delete[] temp_arrays[i];
	}
	
	delete[] temp_arrays;
	
	// Finalize
	MPI_Finalize();
	return 0;
}

