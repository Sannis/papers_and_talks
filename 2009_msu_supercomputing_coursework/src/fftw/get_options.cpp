#include <cstring>

#ifndef _GNU_SOURCE
#	define _GNU_SOURCE
#endif
#include <getopt.h>

// Global defines
#define VERSION "0.2"
#define AUTHOR "Evgeniya Smetanina"
#define BUGREPORT_EMAIL "jannes-2002@yandex.ru"

extern int N, steps_between_saves;
extern double Lx, R, z_final, dz;
extern char results_root_path[];
extern int fftw_no_buffer_flag, fftw_transpose_flag, fftw_measure_flag;

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
	printf("FFT(using FFTW) + simple nonlinearity method.\n");

	printf("\nOptions:\n");
	printf("  -n    number of grid points in cross-section\n");
	printf("  -l    half-width of impulse\n\n");

	//printf("  --impulse-file=<filename>    file with initial conditions, complex impulse shape\n");
	printf("  --results-root=<path>        path for results directories\n\n");
	
	printf("  --fftw-no-buffer        don't use temporary buffer to increase FFTW productivity\n");
	printf("  --fftw-do-transpose        do transpose on each FFT transform by FFTW\n\n");
	printf("  --fftw-do-measure        use measure on FFTW plan creation for increase speed\n\n");

	printf("  --z-final=<distance>    propagation distance\n");
	printf("  -z    step by z-axis\n");
	printf("  -r    Kerr koefficient\n");
	printf("  -d    number of steps between saving data\n\n");
	
	printf("  --debug    displays debug information and save data to ./results/debug\n");
	printf("  -h    displays this stupid help information\n");
	printf("  -v    displays program version\n");
	
	printf("\nExamples:\n");
	printf("user@skif> mpirun -np 16 %s -n 512\n", program_name);
	printf("user@fen1> mpisubmit.bg -n 32 %s -- -n 1024 -l 10 -r 5\n", program_name);
	
	printf("\nAuthor: %s.\n", AUTHOR);
	printf("Report bugs to <%s>.\n", BUGREPORT_EMAIL);
}
//---------------------------------------------------------------------------
void display_version(const char *argv0)
{
	printf("%s version %s\n", get_program_name(argv0), VERSION);
	printf("MPI program for impulse propagation research considering diffraction and Kerr nonlinearity.\n");
	printf("FFT(using FFTW) + simple nonlinearity method.\n");
	
	printf("\nAuthor: %s.\n", AUTHOR);
	printf("Report bugs to <%s>.\n", BUGREPORT_EMAIL);
}
//---------------------------------------------------------------------------
void get_program_options(int argc, char *argv[])
{
	int c = 0;
	opterr = 0;
	
	static struct option long_options[] = {
		{"results-root", required_argument, NULL, 0},
		{"fftw-no-buffer", no_argument, NULL, 0},
		{"fftw-do-transpose", no_argument, NULL, 0},
		{"fftw-do-measure", no_argument, NULL, 0},
		{"z-final", required_argument, NULL, 0},
		{"help", no_argument, NULL, 'h'},
		{"version", no_argument, NULL, 'v'}
	};
	int option_index = 0;
	
	while ( (c = getopt_long(argc, argv, "n:l:r:z:d:hv", long_options, &option_index)) != -1 ) {
		switch (c) {
			case 0:
				if(         strcmp(long_options[option_index].name, "results-root") == 0 ) {
					strcpy(results_root_path, optarg);
				} else if( strcmp(long_options[option_index].name, "fftw-no-buffer") == 0 ) {
					fftw_no_buffer_flag = 1;
				} else if( strcmp(long_options[option_index].name, "fftw-do-transpose") == 0 ) {
					fftw_transpose_flag = 1;
				} else if( strcmp(long_options[option_index].name, "fftw-do-measure") == 0 ) {
					fftw_measure_flag = 1;
				} else if( strcmp(long_options[option_index].name, "z-final") == 0 ) {
					z_final = atof(optarg);
				}
				break;
				
			case 'n':
				N = atoi(optarg);
				break;
			case 'l':
				Lx = atof(optarg);
				break;
			case 'r':
				R = atof(optarg);
				break;
			case 'z':
				dz = atof(optarg);
				break;
			case 'd':
				steps_between_saves = atoi(optarg);
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
	
	if( results_root_path[0] == '\0' ) {
	    printf("ERROR: You must specify root path for results.\n");
	    display_help(argv[0]);
	    exit(0);
	}
}
