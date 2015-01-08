#ifndef PARAM_STRUCT_H
#define PARAM_STRUCT_H

struct param_struct
{
	// Data parameters
	int n;
	int steps_between_saves;
	
	// Propogation parameters
	char* filename_input;	// Impulse file name
	double l;		// Impulse halfwidth
	double z_final;	// Propagation distance
	double dz;		// Step by Z axis for methods with constant step
	double R;		// Kerr nonlinearity coefficient
	
	// MPI processors grid sizes
	int npx;
	int npy;
	
	// Debug flag
	int debug;
	
	// Path to results dir
	char* results_root_path;
	
	// Number of OpenMP threads available
	/// TODO: Implement this
	//int openmp_threads_num;
};

#endif

