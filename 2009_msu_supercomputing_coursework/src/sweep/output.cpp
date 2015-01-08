#include <cstdio>

using namespace std;

extern MPI_Datatype filetype_darray_complex, filetype_darray_double;

//==========//============//==========//==========//==========//

template <class T>
inline T sqr(T x)
{
	return x*x;
}

int OutComplexBinary(complex<double> *arr, long int len, double dist, char *mode, char *file_name)
{
	long int i;
	double tmp;
	FILE *p_f;
	p_f = fopen(file_name, "wb");
	if (mode == "a")
		for (i = 0; i < len; i++) 
		{
			tmp = abs(arr[i]);
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "p")
		for (i = 0; i < len; i++)
		{
			tmp = arg(arr[i]);
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "r")
		for (i = 0; i < len; i++)
		{
			tmp = arr[i].real();
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "i")
		for (i = 0; i < len; i++)
		{	
			tmp = arr[i].imag();
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "n")
		for (i = 0; i < len; i++)
		{
			tmp = norm(arr[i]);
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "ap")
		for (i = 0; i < len; i++)
		{
			tmp = abs(arr[i]);
			fwrite(&tmp, sizeof(double), 1, p_f);
			tmp = arg(arr[i]);
			fwrite(&tmp, sizeof(double), 1, p_f);
		}
	else if (mode == "ri")
		for (i = 0; i < len; i++) fwrite(arr, sizeof(double), 2*len, p_f);
	else
	{	
		fclose(p_f);
		return -1;
	}

	fwrite(&dist, sizeof(double), 1, p_f);
	fclose(p_f);
	return 0;
}

int OutComplexText(complex<double> *arr, long int len, double dist, char *mode, char *file_name)
{
	long int i;
	double tmp;
	FILE *p_f;
	p_f = fopen(file_name, "wt");
	
	if (mode == "a")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n", abs(arr[i]));
	else if (mode == "p")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n", arg(arr[i]));
	else if (mode == "r")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n", arr[i].real());
	else if (mode == "i")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n", arr[i].imag());
	else if (mode == "n")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n", norm(arr[i]));
	else if (mode == "ap")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n%f\n", abs(arr[i]), arg(arr[i]));
	else if (mode == "ri")
		for (i = 0; i < len; i++) fprintf(p_f, "%f\n%f\n", arr[i].real(), arr[i].imag());
	else
	{
		fclose(p_f);	
		return -1;
	}

	fprintf(p_f, "%f\n", dist);
	fclose(p_f);
	return 0;
}

void OutComplexBinaryParallel(MPI_Comm comm, complex<double> *arr, long int len, char *file_name)
{
	MPI_File p_f;
	MPI_File_open(comm, file_name, MPI_MODE_CREATE | MPI_MODE_WRONLY, MPI_INFO_NULL, &p_f);
	MPI_File_set_view(p_f, 0, MPI_DOUBLE_COMPLEX, filetype_darray_complex, "native", MPI_INFO_NULL);
	MPI_File_write_all(p_f, arr, len, MPI_DOUBLE_COMPLEX, MPI_STATUS_IGNORE);
	MPI_File_close(&p_f);
}
