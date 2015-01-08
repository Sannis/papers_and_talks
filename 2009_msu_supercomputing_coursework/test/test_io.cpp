#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mpi.h>

#include <getopt.h>

#include "sns_structs.h"
#include "sns_util_fs.h"

#define AUTHOR "Oleg Efimov"
#define BUGREPORT_EMAIL "efimovov@gmail.com"

#define FILENAME_LEN 512

char* get_program_name(char *argv0)
{
	char* ch = strrchr(argv0, '/');
		if ( ch != NULL ) {
		return ch + 1;
	} else {
		return argv0;
	}
}

void display_help(char *argv0)
{
	char *program_name = get_program_name(argv0);
	printf("Usage: %s [options]\n", program_name);
	printf("Test utility for parallel DArray file output.\n");
	printf("MPI runtime environment needed for execute.\n");
	
	printf("\nOptions:\n");
	printf(" -n           grid size\n");
	printf(" --npx, --npy number of processes in array distribution by X and Y axis\n");
	printf(" --input      name of input file\n");
	
	printf(" -h, --help   display this help and exit\n");
	
	printf("\nExamples:\n");
	printf("mpirun -np 4 %s -n 512 --npx=2 --npy=2\n", program_name);
	printf("mpirun -np 32 %s -n 512 --npx=8 --npy=4\n", program_name);
	
	printf("\nAuthor: %s.\n", AUTHOR);
	printf("Report bugs to <%s>.\n", BUGREPORT_EMAIL);
}

void get_program_options(int argc, char *argv[], program_parameters *p_parameters)
{
	int c = 0;
	opterr = 0;
	
	static struct option long_options[] = {
		{"npx", required_argument, NULL, 0},
		{"npy", required_argument, NULL, 0},
		{"input", required_argument, NULL, 0},
		{"debug", no_argument, NULL, 'd'},
		{"help", no_argument, NULL, 'h'}
	};
	int option_index = 0;

	while ( (c = getopt_long_only(argc, argv, "n:", long_options, &option_index)) != -1 ) {
		switch(c) {
			case 0:
				if(        strcmp(long_options[option_index].name, "npx") == 0 ) {
					sscanf(optarg, "%d", &p_parameters->npx);
				} else if( strcmp(long_options[option_index].name, "npy") == 0 ) {
					sscanf(optarg, "%d", &p_parameters->npy);
				} else if( strcmp(long_options[option_index].name, "input") == 0 ) {
					strcpy(p_parameters->filename_input, optarg);
				}
				break;

			case 'n':
				sscanf(optarg, "%d", &p_parameters->n);
				break;

			case 'd':
				p_parameters->debug = true;
				break;

			case 'h':
				display_help(argv[0]);
				exit(0);
				break;

			case '?':
				printf("Unknown option %d.\n", optopt);
				break;
			
			default:
				break;
		}
	}
}

void init_arr(double *array, struct param_mpi_data *p_mpi_data)
{
	int i, j, glob_i, glob_j;
	
	for( i = 0; i < p_mpi_data->local_ny; i++ )
	{		
		for( j = 0; j < p_mpi_data->local_nx; j++ )
		{
			array[i*p_mpi_data->local_nx + j] = p_mpi_data->mpi_rank;
		}
	}
}

int main(int argc, char **argv)
{
	// Variables
	program_parameters p_parameters;
	param_mpi_data p_mpi_data;
	double *local_data;
	
	// Default parameters
	p_parameters.n = 256;
	p_parameters.npx = 4;
	p_parameters.npy = 8;
	p_parameters.filename_input = new char[FILENAME_LEN];
	p_parameters.filename_input[0] = '\0';
	p_parameters.debug = false;
	
	// Initialize MPI, get process rank
	MPI_Init(&argc, &argv);
	p_mpi_data.mpi_comm = MPI_COMM_WORLD;
	MPI_Comm_rank(p_mpi_data.mpi_comm, &p_mpi_data.mpi_rank);
	MPI_Comm_size(p_mpi_data.mpi_comm, &p_mpi_data.mpi_size);

	// Parse program command line options
	if( p_mpi_data.mpi_rank == 0 ){
		get_program_options(argc, argv, &p_parameters);

		p_mpi_data.n = p_parameters.n;
		p_mpi_data.npx = p_parameters.npx;
		p_mpi_data.npy = p_parameters.npy;
		
		// Check input parameters
		if( p_mpi_data.mpi_size != p_parameters.npx*p_parameters.npy ){
			printf("ERROR: Wrong processes number!\n");
			printf("%d != %dx%d.\n\n", p_mpi_data.mpi_size, p_parameters.npx, p_parameters.npy);
			MPI_Abort(p_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return 1;
		}
		
		// Some debug output
		printf("Log: MPI processes: %d.\n", p_mpi_data.mpi_size);
		printf("%dx%d matrix will be distributed by %dx%d processes grid.\n", p_mpi_data.n, p_mpi_data.n, p_mpi_data.npx, p_mpi_data.npy);
	}
	
	printf("Log[%.2d]: Initialization				OK.\n", p_mpi_data.mpi_rank);
	
	// Broadcast parameters from root to all processes
	MPI_Bcast(&p_mpi_data.n, 1, MPI_INTEGER, 0, p_mpi_data.mpi_comm);
	MPI_Bcast(&p_mpi_data.npx, 1, MPI_INTEGER, 0, p_mpi_data.mpi_comm);
	MPI_Bcast(&p_mpi_data.npy, 1, MPI_INTEGER, 0, p_mpi_data.mpi_comm);
	MPI_Bcast(p_parameters.filename_input, FILENAME_LEN, MPI_CHAR, 0, p_mpi_data.mpi_comm);
	MPI_Bcast(&p_parameters.debug, 1, MPI_BYTE, 0, p_mpi_data.mpi_comm); /// FIXME

	printf("Log[%.2d]: MPI_Bcast				OK.\n", p_mpi_data.mpi_rank);
	
	// Calculate local sizes
	p_mpi_data.local_nx = p_mpi_data.n/p_mpi_data.npx;
	p_mpi_data.local_ny = p_mpi_data.n/p_mpi_data.npy;

	if(p_parameters.debug) {
		printf("[%.2d]: n = %d.\n", p_mpi_data.mpi_rank, p_mpi_data.n);
		printf("[%.2d]: npx = %d.\n", p_mpi_data.mpi_rank, p_mpi_data.npx);
		printf("[%.2d]: npy = %d.\n", p_mpi_data.mpi_rank, p_mpi_data.npy);
		printf("[%.2d]: local_nx = %d.\n", p_mpi_data.mpi_rank, p_mpi_data.local_nx);
		printf("[%.2d]: local_ny = %d.\n", p_mpi_data.mpi_rank, p_mpi_data.local_ny);
		printf("[%.2d]: filename_input = %s.\n", p_mpi_data.mpi_rank, p_parameters.filename_input);
	}
	
	
	
	// Allocate local part of matrix
	local_data = new double[p_mpi_data.local_nx*p_mpi_data.local_ny];
	if( !local_data ) {
		printf("Log[%.2d]: new double				FAILED.\n", p_mpi_data.mpi_rank);
		MPI_Abort(p_mpi_data.mpi_comm, MPI_ERR_OTHER);
		return 1;
	}

	printf("Log[%.2d]: new double				OK.\n", p_mpi_data.mpi_rank);
	
	// Initialize local part of matrix
	if( p_parameters.filename_input[0] != '\0' ) {
		if( load_darray(local_data, &p_mpi_data, MPI_DOUBLE, p_parameters.filename_input) != MPI_SUCCESS ) {
			printf("Log[%.2d]: Reading input file                FAILED.\n", p_mpi_data.mpi_rank);
			MPI_Abort(p_mpi_data.mpi_comm, MPI_ERR_OTHER);
			return 1;
		}
		printf("Log[%.2d]: Reading input file			OK.\n", p_mpi_data.mpi_rank);
	} else {
		init_arr(local_data, &p_mpi_data);
		printf("Log[%.2d]: Array initialization			OK.\n", p_mpi_data.mpi_rank);
	}

	
	// Save matrix
	char filename[FILENAME_LEN];
	sprintf(filename, "test_%d_%dx%d.bin", p_mpi_data.n, p_mpi_data.npx, p_mpi_data.npy);
	if( save_darray(local_data, &p_mpi_data, MPI_DOUBLE, filename) != MPI_SUCCESS ) {
		printf("Log[%.2d]: Writing output file                FAILED.\n", p_mpi_data.mpi_rank);
		MPI_Abort(p_mpi_data.mpi_comm, MPI_ERR_OTHER);
		return 1;
	}
	
	printf("Log[%.2d]: Writing output file			OK.\n", p_mpi_data.mpi_rank);

	// Free memory
	delete[] local_data;
	delete[] p_parameters.filename_input;
	
	// Finalize
	MPI_Finalize();
	return 0;
}
