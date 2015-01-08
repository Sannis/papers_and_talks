#include "sns_util_fs.h"

/**
* @todo	Write doccomment
*/
int save_darray(void *local_data, struct param_mpi_data *p_mpi_data, MPI_Datatype datatype, char* filename)
{
	int distribs[2], dargs[2], gsizes[2], psizes[2];
	distribs[0] = MPI_DISTRIBUTE_BLOCK;
	distribs[1] = MPI_DISTRIBUTE_BLOCK;
	dargs[0] = MPI_DISTRIBUTE_DFLT_DARG;
	dargs[1] = MPI_DISTRIBUTE_DFLT_DARG;
	gsizes[0] = p_mpi_data->n;
	gsizes[1] = p_mpi_data->n;
	psizes[0] = p_mpi_data->npy;
	psizes[1] = p_mpi_data->npx;

	MPI_File fp;
	MPI_Datatype filetype;
	MPI_Status status;

	MPI_Type_create_darray(p_mpi_data->mpi_size, p_mpi_data->mpi_rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, datatype, &filetype);
	MPI_Type_commit(&filetype);

	MPI_File_open(p_mpi_data->mpi_comm, filename, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &fp);
	MPI_File_set_view(fp, 0, datatype, filetype, "native", MPI_INFO_NULL);

	if( MPI_File_write_all(fp, local_data, p_mpi_data->local_nx*p_mpi_data->local_ny, datatype, &status) != MPI_SUCCESS ) {
		MPI_Abort(p_mpi_data->mpi_comm, MPI_ERR_OTHER);
		return MPI_ERR_OTHER;
	}

	MPI_File_close(&fp);
	MPI_Type_free(&filetype);
	return MPI_SUCCESS;
}

/**
* @todo	Write doccomment
*/
int load_darray(void *local_data, struct param_mpi_data *p_mpi_data, MPI_Datatype datatype, char* filename)
{
	int distribs[2], dargs[2], gsizes[2], psizes[2];
	distribs[0] = MPI_DISTRIBUTE_BLOCK;
	distribs[1] = MPI_DISTRIBUTE_BLOCK;
	dargs[0] = MPI_DISTRIBUTE_DFLT_DARG;
	dargs[1] = MPI_DISTRIBUTE_DFLT_DARG;
	gsizes[0] = p_mpi_data->n;
	gsizes[1] = p_mpi_data->n;
	psizes[0] = p_mpi_data->npy;
	psizes[1] = p_mpi_data->npx;

	MPI_File fp;
	MPI_Datatype filetype;
	MPI_Status status;

	MPI_Type_create_darray(p_mpi_data->mpi_size, p_mpi_data->mpi_rank, 2, gsizes, distribs, dargs, psizes, MPI_ORDER_C, datatype, &filetype);
	MPI_Type_commit(&filetype);

	MPI_File_open(p_mpi_data->mpi_comm, filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &fp);
	MPI_File_set_view(fp, 0, datatype, filetype, "native", MPI_INFO_NULL);

	if( MPI_File_read_all(fp, local_data, p_mpi_data->local_nx*p_mpi_data->local_ny, datatype, &status) != MPI_SUCCESS ) {
		MPI_Abort(p_mpi_data->mpi_comm, MPI_ERR_OTHER);
		return MPI_ERR_OTHER;
	}

	MPI_File_close(&fp);
	MPI_Type_free(&filetype);
	return MPI_SUCCESS;
}

