#include <mpi.h>

#include "sns_structs.h"

/**
* @todo	Write doccomment
*/
int save_darray(void *local_data, struct param_mpi_data *p_mpi_data, MPI_Datatype datatype, char* filename);

/**
* @todo	Write doccomment
*/
int load_darray(void *local_data, struct param_mpi_data *p_mpi_data, MPI_Datatype datatype, char* filename);
