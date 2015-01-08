extern complex<double> *EEE;

extern double *dx, *dy;
extern complex<double> *sweep_a_x, *sweep_b_x, *sweep_c_x, *sweep_f_x;
extern complex<double> *sweep_a_y, *sweep_b_y, *sweep_c_y, *sweep_f_y;
extern complex<double> *buf_x, *buf_y;
extern complex<double> *edge_l, *edge_r, *edge_t, *edge_b;
extern complex<double> *send_x, *send_y;

extern MPI_Datatype filetype_darray_complex, filetype_darray_double;
extern MPI_Comm comm_cart, comm_row, comm_col;

//==========//============//==========//==========//==========//

void Finalize()
{
	delete[] sweep_a_x;
	delete[] sweep_b_x;
	delete[] sweep_c_x;
	delete[] sweep_f_x;
	delete[] buf_x;
	delete[] sweep_a_y;
	delete[] sweep_b_y;
	delete[] sweep_c_y;
	delete[] sweep_f_y;
	delete[] buf_y;
	delete[] edge_l;
	delete[] edge_r;
	delete[] edge_t;
	delete[] edge_b;
	delete[] send_x;
	delete[] send_y;

	MPI_Type_free(&filetype_darray_complex);
	MPI_Type_free(&filetype_darray_double);
	MPI_Comm_free(&comm_row);
	MPI_Comm_free(&comm_col);

	delete[] EEE;

	delete[] dx;
	delete[] dy;
}
