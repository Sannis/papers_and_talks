#ifndef SNS_STRUCTS_H
#define SNS_STRUCTS_H

	struct program_parameters
	{
		int npx;
		int npy;
		
		int n;
		
		char* filename_input;
		
		bool debug;
	};

	struct param_mpi_data
	{
		MPI_Comm mpi_comm;
		
		int mpi_size;
		int mpi_rank;
 		
		int npx;
		int npy;
		
		int n;
        	
		int local_nx;
		int local_ny;
	};
#endif
