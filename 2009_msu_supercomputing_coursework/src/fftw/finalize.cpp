extern double *x;
extern double *y;
extern fftwnd_mpi_plan p,p2;
extern complex<double> *local_data;
extern double *output;

void Finalize()
{
	delete[] x;
	delete[] y;
	delete[] local_data;
	delete[] output;
	fftwnd_mpi_destroy_plan(p);
	fftwnd_mpi_destroy_plan(p2);
	MPI_Finalize();
}
