#include "sns_mpi.h"

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
		int save_darray(void *local_data, struct_mpi_data *s_mpi_data, MPI_Datatype datatype, char* filename)
		{
			int distribs[2], dargs[2], gsizes[2], psizes[2];
			distribs[0] = MPI_DISTRIBUTE_BLOCK;
			distribs[1] = MPI_DISTRIBUTE_BLOCK;
			dargs[0] = MPI_DISTRIBUTE_DFLT_DARG;
			dargs[1] = MPI_DISTRIBUTE_DFLT_DARG;
			gsizes[0] = s_mpi_data->n;
			gsizes[1] = s_mpi_data->n;
			psizes[0] = s_mpi_data->npy;
			psizes[1] = s_mpi_data->npx;

			MPI_File fp;
			MPI_Datatype filetype;
			MPI_Status status;

			MPI_Type_create_darray(s_mpi_data->mpi_size, s_mpi_data->mpi_rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, datatype, &filetype);
			MPI_Type_commit(&filetype);

			MPI_File_open(s_mpi_data->mpi_comm, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
			MPI_File_set_view(fp, 0, datatype, filetype, "native", MPI_INFO_NULL);

			if( MPI_File_write_all(fp, local_data, s_mpi_data->local_nx*s_mpi_data->local_ny, datatype, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
				return MPI_ERR_OTHER;
			}

			MPI_File_close(&fp);
			MPI_Type_free(&filetype);
			return MPI_SUCCESS;
		}

		/**
		* @todo	Write doccomment
		*/
		int load_darray(void *local_data, struct_mpi_data *s_mpi_data, MPI_Datatype datatype, char* filename)
		{
			int distribs[2], dargs[2], gsizes[2], psizes[2];
			distribs[0] = MPI_DISTRIBUTE_BLOCK;
			distribs[1] = MPI_DISTRIBUTE_BLOCK;
			dargs[0] = MPI_DISTRIBUTE_DFLT_DARG;
			dargs[1] = MPI_DISTRIBUTE_DFLT_DARG;
			gsizes[0] = s_mpi_data->n;
			gsizes[1] = s_mpi_data->n;
			psizes[0] = s_mpi_data->npy;
			psizes[1] = s_mpi_data->npx;

			MPI_File fp;
			MPI_Datatype filetype;
			MPI_Status status;

			MPI_Type_create_darray(s_mpi_data->mpi_size, s_mpi_data->mpi_rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, datatype, &filetype);
			MPI_Type_commit(&filetype);

			MPI_File_open(s_mpi_data->mpi_comm, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fp);
			MPI_File_set_view(fp, 0, datatype, filetype, "native", MPI_INFO_NULL);

			if( MPI_File_read_all(fp, local_data, s_mpi_data->local_nx*s_mpi_data->local_ny, datatype, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
				return MPI_ERR_OTHER;
			}

			MPI_File_close(&fp);
			MPI_Type_free(&filetype);
			return MPI_SUCCESS;
		}
		
		/**
		* @todo	Write doccomment
		*/
		double calc_intensity_integral(std::complex<double> *E, struct_mpi_data *s_mpi_data)
		{
			double local_intensity = 0, intensity = 0;
			int i = 0;

			for( i = 0; i < s_mpi_data->total_local_size; i++ )
			{
				local_intensity += norm(E[i]);
			}

			MPI_Allreduce(&local_intensity, &intensity, 1, MPI_DOUBLE, MPI_SUM, s_mpi_data->mpi_comm);

			return intensity;
		}

		/**
		* @todo	Write doccomment
		*/
		double calc_intensity_maximum(std::complex<double> *E, struct_mpi_data *s_mpi_data)
		{
			double local_intensity_maximum = 0, intensity_maximum = 0;
			double intensity = 0;
			int i = 0;

			//#pragma omp parallel for reduction(max:local_intensity_maximum)
			// For future: Yes, C/C++ OpenMP library doesn't support "max" operation for reduce
			for( i = 0; i < s_mpi_data->total_local_size; i++ )
			{
				intensity = norm(E[i]);
				if( intensity > local_intensity_maximum ) {
					local_intensity_maximum = intensity;
				}
			}

			MPI_Allreduce(&local_intensity_maximum, &intensity_maximum, 1, MPI_DOUBLE, MPI_MAX, s_mpi_data->mpi_comm);

			return intensity_maximum;
		}
	}
}
