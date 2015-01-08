#ifndef SNS_MPI_H
#define SNS_MPI_H

#include <complex>

#define MPICH_IGNORE_CXX_SEEK
#include <mpi.h>

/**
* @todo	Write doccomment
*/
namespace sns
{
	/**
	* @todo	Write doccomment
	*/
	namespace mpi
	{
		/**
		* @todo	Write doccomment
		*/
		struct struct_mpi_data
		{
			MPI_Comm mpi_comm;
		
			int mpi_size;
			int mpi_rank;
	 		
			int npx;
			int npy;
		
			int n;
			
			int local_nx;
			int local_ny;
			int total_local_size;
		};
		
		/**
		* @todo	Write doccomment
		*/
		int save_darray(void *local_data, struct_mpi_data *s_mpi_data, MPI_Datatype datatype, char* filename);

		/**
		* @todo	Write doccomment
		*/
		int load_darray(void *local_data, struct_mpi_data *s_mpi_data, MPI_Datatype datatype, char* filename);
		
		/**
		* @todo	Write doccomment
		*/
		double calc_intensity_integral(std::complex<double> *E, struct_mpi_data *s_mpi_data);

		/**
		* @todo	Write doccomment
		*/
		double calc_intensity_maximum(std::complex<double> *E, struct_mpi_data *s_mpi_data);
	}
}

#endif

