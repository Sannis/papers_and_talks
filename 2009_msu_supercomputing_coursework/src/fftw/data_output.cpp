
extern char results_path[];
extern char filename_binary[];
extern MPI_File fh;
extern MPI_Status status;
extern int N;
extern double *x;
extern double *y;
extern double Lx, dx,dy, dz;
extern int iz;
extern complex<double> *local_data;
extern int n_fields ;
extern int local_nx,
	local_x_start,
	local_ny_after_transpose,
	local_y_start_after_transpose,
	total_local_size;
extern int size, rank;

void Data_Output(int iz)
{
// параллельный вывод
	// контрольный вывод
	//printf("\n rank %d ;step z %d ; %lf %lf %lf\n", rank,iz,output[N/2],x[local_x_start], y[N/2]);

	//printf("ok\n");

	sprintf(filename_binary,"%s/out_%05d.cpl", results_path, iz);

	MPI_File_open(MPI_COMM_WORLD, 
			filename_binary,
			MPI_MODE_RDWR | MPI_MODE_CREATE,
			MPI_INFO_NULL, 
			&fh 
			);

	MPI_File_set_view(fh, 0 , MPI_DOUBLE_COMPLEX, MPI_DOUBLE_COMPLEX, "native", MPI_INFO_NULL);
	MPI_File_write_at(fh, local_x_start*N, local_data, total_local_size, MPI_DOUBLE_COMPLEX, &status);
	MPI_File_close(&fh);
}

 void Write_Log(char* filename, char* log)
{
	FILE *fp = fopen(filename, "a");
	 
	if ( !fp ) {
		printf("ERROR: Can't write program execution log. Error code: 1.\n");
		return;
	}
	 
	if ( fputs(log, fp) == EOF ) {
		printf("ERROR: Can't write program execution log. Error code: 2.\n");
	}
	 
	fclose(fp);
}
