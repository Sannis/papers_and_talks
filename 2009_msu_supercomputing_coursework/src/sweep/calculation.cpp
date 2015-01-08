extern complex<double> _COMPLEX_I;

extern double kerrR;

extern double *dx, *dy;
extern complex<double> *buf_x, *sweep_a_x, *sweep_b_x, *sweep_c_x, *sweep_f_x;
extern complex<double> *buf_y, *sweep_a_y, *sweep_b_y, *sweep_c_y, *sweep_f_y;
extern complex<double> *edge_l, *edge_r, *edge_t, *edge_b;
extern complex<double> *send_x, *send_y;

extern int rank_row, rank_col, rank;
extern MPI_Comm comm_row, comm_col;
extern int psizes[2];
extern int local_sizes[2];

//==========//============//==========//==========//==========//

void Diffraction(complex<double> *Arr, double z_step)
{
	int i, j, k;	
	complex<double> ga, de;
	complex<double> tmp_x, tmp_y, tmp_u;
	MPI_Status st;
	MPI_Request req_r, req_l, req_t, req_b, req_x, req_y, req_u;

	if (rank_row == 0)
	{
		for (k = 1; k < local_sizes[1]; k++) sweep_c_x[k] = sweep_a_x[k] + sweep_b_x[k] + _COMPLEX_I*(dx[k] + dx[k + 1])/z_step;

		MPI_Irecv(edge_r, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row + 1, 5, comm_row, &req_r);
		for (k = 0; k < local_sizes[0]; k++) send_x[k] = Arr[(k + 1)*local_sizes[1] - 1];
		MPI_Send(send_x, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row + 1, 6, comm_row);
	
		MPI_Wait(&req_r, &st);
	}
	else if (rank_row == psizes[1] - 1)
	{
		ga = 1./(1. + _COMPLEX_I*2.*sqr(dx[local_sizes[1] - 1])/z_step);
		for (k = 0; k < local_sizes[1] - 1; k++) sweep_c_x[k] = sweep_a_x[k] + sweep_b_x[k] + _COMPLEX_I*(dx[k] + dx[k + 1])/z_step;

		MPI_Irecv(edge_l, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row - 1, 6, comm_row, &req_l);
		for (k = 0; k < local_sizes[0]; k++) send_x[k] = Arr[k*local_sizes[1]];
		MPI_Send(send_x, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row - 1, 5, comm_row);

		MPI_Wait(&req_l, &st);
	}
	else
	{
		for (k = 0; k < local_sizes[1]; k++) sweep_c_x[k] = sweep_a_x[k] + sweep_b_x[k] + _COMPLEX_I*(dx[k] + dx[k + 1])/z_step;

		MPI_Irecv(edge_l, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row - 1, 6, comm_row, &req_l);
		MPI_Irecv(edge_r, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row + 1, 5, comm_row, &req_r);
		for (k = 0; k < local_sizes[0]; k++) send_x[k] = Arr[(k + 1)*local_sizes[1] - 1];
		MPI_Send(send_x, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row + 1, 6, comm_row);
		for (k = 0; k < local_sizes[0]; k++) send_x[k] = Arr[k*local_sizes[1]];
		MPI_Send(send_x, local_sizes[0], MPI_DOUBLE_COMPLEX, rank_row - 1, 5, comm_row);

		MPI_Wait(&req_r, &st);
		MPI_Wait(&req_l, &st);
	}

	for (int sw_tact = 0; sw_tact < local_sizes[0] + psizes[1] - 1; sw_tact++)
	{
		bool flag1 = ((rank_row <= sw_tact) && (rank_row >= sw_tact - local_sizes[0] + 1));
		bool flag2 = ((rank_row > 0) && (rank_row <= sw_tact + 1) && (rank_row >= sw_tact - local_sizes[0] + 2));

		if (flag1)
		{
			if (rank_row == 0)
			{
				for (k = 1; k < local_sizes[1] - 1; k++) sweep_f_x[k] = sweep_b_x[k]*Arr[sw_tact*local_sizes[1] + k + 1]
													- (sweep_a_x[k] + sweep_b_x[k])*Arr[sw_tact*local_sizes[1] + k]
													+ sweep_a_x[k]*Arr[sw_tact*local_sizes[1] + k - 1]
													+ (sweep_c_x[k] - sweep_a_x[k] - sweep_b_x[k])*Arr[sw_tact*local_sizes[1] + k];
				sweep_f_x[local_sizes[1] - 1] = sweep_b_x[local_sizes[1] - 1]*edge_r[sw_tact]
									- (sweep_a_x[local_sizes[1] - 1] + sweep_b_x[local_sizes[1] - 1])*Arr[sw_tact*local_sizes[1] + local_sizes[1] - 1]
									+ sweep_a_x[local_sizes[1] - 1]*Arr[sw_tact*local_sizes[1] + local_sizes[1] - 2]
									+ (sweep_c_x[local_sizes[1] - 1] - sweep_a_x[local_sizes[1] - 1] - sweep_b_x[local_sizes[1] - 1])*Arr[sw_tact*local_sizes[1] + local_sizes[1] - 1];

				buf_x[sw_tact*local_sizes[1]] = 1./(1. + _COMPLEX_I*2.*sqr(dx[1])/z_step);
				buf_y[sw_tact*local_sizes[1]] = (Arr[sw_tact*local_sizes[1] + 1] - Arr[sw_tact*local_sizes[1]]*(1. - _COMPLEX_I*2.*sqr(dx[1])/z_step))/(1. + _COMPLEX_I*2.*sqr(dx[1])/z_step);
			}
			else if (rank_row == psizes[1] - 1)
			{
				sweep_f_x[0] = sweep_b_x[0]*Arr[(sw_tact - rank_row)*local_sizes[1] + 1]
							- (sweep_a_x[0] + sweep_b_x[0])*Arr[(sw_tact - rank_row)*local_sizes[1]]
							+ sweep_a_x[0]*edge_l[sw_tact - rank_row]
							+ (sweep_c_x[0] - sweep_a_x[0] - sweep_b_x[0])*Arr[(sw_tact - rank_row)*local_sizes[1]];
				for (k = 1; k < local_sizes[1] - 1; k++) sweep_f_x[k] = sweep_b_x[k]*Arr[(sw_tact - rank_row)*local_sizes[1] + k + 1]
													- (sweep_a_x[k] + sweep_b_x[k])*Arr[(sw_tact - rank_row)*local_sizes[1] + k]
													+ sweep_a_x[k]*Arr[(sw_tact - rank_row)*local_sizes[1] + k - 1]
													+ (sweep_c_x[k] - sweep_a_x[k] - sweep_b_x[k])*Arr[(sw_tact - rank_row)*local_sizes[1] + k];

				buf_x[(sw_tact - rank_row)*local_sizes[1]] = sweep_b_x[0]/(sweep_c_x[0] - sweep_a_x[0]*tmp_x);
				buf_y[(sw_tact - rank_row)*local_sizes[1]] = (sweep_f_x[0] + sweep_a_x[0]*tmp_y)/(sweep_c_x[0] - sweep_a_x[0]*tmp_x);
			}
			else
			{
				sweep_f_x[0] = sweep_b_x[0]*Arr[(sw_tact - rank_row)*local_sizes[1] + 1]
							- (sweep_a_x[0] + sweep_b_x[0])*Arr[(sw_tact - rank_row)*local_sizes[1]]
							+ sweep_a_x[0]*edge_l[sw_tact - rank_row]
							+ (sweep_c_x[0] - sweep_a_x[0] - sweep_b_x[0])*Arr[(sw_tact - rank_row)*local_sizes[1]];
				for (k = 1; k < local_sizes[1] - 1; k++) sweep_f_x[k] = sweep_b_x[k]*Arr[(sw_tact - rank_row)*local_sizes[1] + k + 1]
													- (sweep_a_x[k] + sweep_b_x[k])*Arr[(sw_tact - rank_row)*local_sizes[1] + k]
													+ sweep_a_x[k]*Arr[(sw_tact - rank_row)*local_sizes[1] + k - 1]
													+ (sweep_c_x[k] - sweep_a_x[k] - sweep_b_x[k])*Arr[(sw_tact - rank_row)*local_sizes[1] + k];
				sweep_f_x[local_sizes[1] - 1] = sweep_b_x[local_sizes[1] - 1]*edge_r[sw_tact - rank_row]
									- (sweep_a_x[local_sizes[1] - 1] + sweep_b_x[local_sizes[1] - 1])*Arr[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1]
									+ sweep_a_x[local_sizes[1] - 1]*Arr[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 2]
									+ (sweep_c_x[local_sizes[1] - 1] - sweep_a_x[local_sizes[1] - 1] - sweep_b_x[local_sizes[1] - 1])*Arr[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1];

				buf_x[(sw_tact - rank_row)*local_sizes[1]] = sweep_b_x[0]/(sweep_c_x[0] - sweep_a_x[0]*tmp_x);
				buf_y[(sw_tact - rank_row)*local_sizes[1]] = (sweep_f_x[0] + sweep_a_x[0]*tmp_y)/(sweep_c_x[0] - sweep_a_x[0]*tmp_x);				
			}
		}

		if (flag2)
		{
			MPI_Irecv(&tmp_x, 1, MPI_DOUBLE_COMPLEX, rank_row - 1, 7, comm_row, &req_x);
			MPI_Irecv(&tmp_y, 1, MPI_DOUBLE_COMPLEX, rank_row - 1, 8, comm_row, &req_y);
		}

		if (flag1)
		{
			for (j = 1; j < local_sizes[1] - 1; j++)
			{
				buf_x[(sw_tact - rank_row)*local_sizes[1] + j] = sweep_b_x[j]/(sweep_c_x[j] - sweep_a_x[j]*buf_x[(sw_tact - rank_row)*local_sizes[1] + j - 1]);
				buf_y[(sw_tact - rank_row)*local_sizes[1] + j] = (sweep_f_x[j] + sweep_a_x[j]*buf_y[(sw_tact - rank_row)*local_sizes[1] + j - 1])/(sweep_c_x[j] - sweep_a_x[j]*buf_x[(sw_tact - rank_row)*local_sizes[1] + j - 1]);
			}

			if (rank_row < psizes[1] - 1)
			{
				buf_x[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1] = sweep_b_x[local_sizes[1] - 1]/(sweep_c_x[local_sizes[1] - 1] - sweep_a_x[local_sizes[1] - 1]*buf_x[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 2]);
				buf_y[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1] = (sweep_f_x[local_sizes[1] - 1] + sweep_a_x[local_sizes[1] - 1]*buf_y[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 2])/(sweep_c_x[local_sizes[1] - 1] - sweep_a_x[local_sizes[1] - 1]*buf_x[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 2]);
			}

			if (rank_row < psizes[1] - 1)
			{
				MPI_Send(&(buf_x[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1]), 1, MPI_DOUBLE_COMPLEX, rank_row + 1, 7, comm_row);
				MPI_Send(&(buf_y[(sw_tact - rank_row)*local_sizes[1] + local_sizes[1] - 1]), 1, MPI_DOUBLE_COMPLEX, rank_row + 1, 8, comm_row);
			}
		}

		if (flag2)
		{
			MPI_Wait(&req_x, &st);
			MPI_Wait(&req_y, &st);
		}
	}

	for (int sw_tact = 0; sw_tact < local_sizes[0] + psizes[1] - 1; sw_tact++)
	{
		bool flag1 = ((rank_row >= psizes[1] - 1 - sw_tact) && (rank_row <= local_sizes[0] + psizes[1] - 2 - sw_tact));
		bool flag2 = ((rank_row < psizes[1] - 1) && (rank_row >= psizes[1] - 2 - sw_tact) && (rank_row <= local_sizes[0] + psizes[1] - 3 - sw_tact));

		if (flag1)
		{
			if (rank_row == psizes[1] - 1)
			{
				de = (Arr[sw_tact*local_sizes[1] + local_sizes[1] - 2] - Arr[sw_tact*local_sizes[1] + local_sizes[1] - 1]*(1. - _COMPLEX_I*2.*sqr(dx[local_sizes[1] - 1])/z_step))*ga;
				Arr[sw_tact*local_sizes[1] + local_sizes[1] - 1] = (ga*buf_y[sw_tact*local_sizes[1] + local_sizes[1] - 2] + de)/(1. - ga*buf_x[sw_tact*local_sizes[1] + local_sizes[1] - 2]);
			}
			else
				Arr[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + local_sizes[1] - 1] = tmp_u*buf_x[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + local_sizes[1] - 1] + buf_y[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + local_sizes[1] - 1];
		}

		if (flag2)
			MPI_Irecv(&tmp_u, 1, MPI_DOUBLE_COMPLEX, rank_row + 1, 9, comm_row, &req_u);

		if (flag1)
		{
			for (j = local_sizes[1] - 2; j >= 0; j--)
				Arr[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + j] = Arr[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + j + 1]*buf_x[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + j] + buf_y[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1] + j];
			
			if (rank_row > 0)
				MPI_Send(&(Arr[(sw_tact + rank_row - psizes[1] + 1)*local_sizes[1]]), 1, MPI_DOUBLE_COMPLEX, rank_row - 1, 9, comm_row);
		}

		if (flag2)
			MPI_Wait(&req_u, &st);
	}

	if (rank_col == 0)
	{
		for (k = 1; k < local_sizes[0]; k++) sweep_c_y[k] = sweep_a_y[k] + sweep_b_y[k] + _COMPLEX_I*(dy[k] + dy[k + 1])/z_step;

		MPI_Irecv(edge_b, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col + 1, 15, comm_col, &req_b);
		for (k = 0; k < local_sizes[1]; k++) send_y[k] = Arr[(local_sizes[0] - 1)*local_sizes[1] + k];
		MPI_Send(send_y, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col + 1, 16, comm_col);
	
		MPI_Wait(&req_b, &st);
	}
	else if (rank_col == psizes[0] - 1)
	{
		ga = 1./(1. + _COMPLEX_I*2.*sqr(dy[local_sizes[0] - 1])/z_step);
		for (k = 0; k < local_sizes[0] - 1; k++) sweep_c_y[k] = sweep_a_y[k] + sweep_b_y[k] + _COMPLEX_I*(dy[k] + dy[k + 1])/z_step;

		MPI_Irecv(edge_t, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col - 1, 16, comm_col, &req_t);
		for (k = 0; k < local_sizes[1]; k++) send_y[k] = Arr[k];
		MPI_Send(send_y, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col - 1, 15, comm_col);

		MPI_Wait(&req_t, &st);
	}
	else
	{
		for (k = 0; k < local_sizes[0]; k++) sweep_c_y[k] = sweep_a_y[k] + sweep_b_y[k] + _COMPLEX_I*(dy[k] + dy[k + 1])/z_step;

		MPI_Irecv(edge_t, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col - 1, 16, comm_col, &req_t);
		MPI_Irecv(edge_b, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col + 1, 15, comm_col, &req_b);
		for (k = 0; k < local_sizes[1]; k++) send_y[k] = Arr[(local_sizes[0] - 1)*local_sizes[1] + k];
		MPI_Send(send_y, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col + 1, 16, comm_col);
		for (k = 0; k < local_sizes[1]; k++) send_y[k] = Arr[k];
		MPI_Send(send_y, local_sizes[1], MPI_DOUBLE_COMPLEX, rank_col - 1, 15, comm_col);

		MPI_Wait(&req_t, &st);
		MPI_Wait(&req_b, &st);
	}

	for (int sw_tact = 0; sw_tact < local_sizes[1] + psizes[0] - 1; sw_tact++)
	{
		bool flag1 = ((rank_col <= sw_tact) && (rank_col >= sw_tact - local_sizes[1] + 1));
		bool flag2 = ((rank_col > 0) && (rank_col <= sw_tact + 1) && (rank_col >= sw_tact - local_sizes[1] + 2));

		if (flag1)
		{
			if (rank_col == 0)
			{
				for (k = 1; k < local_sizes[0] - 1; k++) sweep_f_y[k] = sweep_b_y[k]*Arr[(k + 1)*local_sizes[1] + sw_tact]
													- (sweep_a_y[k] + sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact]
													+ sweep_a_y[k]*Arr[(k - 1)*local_sizes[1] + sw_tact]
													+ (sweep_c_y[k] - sweep_a_y[k] - sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact];
				sweep_f_y[local_sizes[0] - 1] = sweep_b_y[local_sizes[0] - 1]*edge_b[sw_tact]
									- (sweep_a_y[local_sizes[0] - 1] + sweep_b_y[local_sizes[0] - 1])*Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact]
									+ sweep_a_y[local_sizes[0] - 1]*Arr[(local_sizes[0] - 2)*local_sizes[1] + sw_tact]
									+ (sweep_c_y[local_sizes[0] - 1] - sweep_a_y[local_sizes[0] - 1] - sweep_b_y[local_sizes[0] - 1])*Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact];

				buf_x[sw_tact] = 1./(1. + _COMPLEX_I*2.*sqr(dy[1])/z_step);
				buf_y[sw_tact] = (Arr[local_sizes[1] + sw_tact] - Arr[sw_tact]*(1. - _COMPLEX_I*2.*sqr(dy[1])/z_step))/(1. + _COMPLEX_I*2.*sqr(dy[1])/z_step);
			}
			else if (rank_col == psizes[0] - 1)
			{
				sweep_f_y[0] = sweep_b_y[0]*Arr[local_sizes[1] + sw_tact - rank_col]
							- (sweep_a_y[0] + sweep_b_y[0])*Arr[sw_tact - rank_col]
							+ sweep_a_y[0]*edge_t[sw_tact - rank_col]
							+ (sweep_c_y[0] - sweep_a_y[0] - sweep_b_y[0])*Arr[sw_tact - rank_col];
				for (k = 1; k < local_sizes[0] - 1; k++) sweep_f_y[k] = sweep_b_y[k]*Arr[(k + 1)*local_sizes[1] + sw_tact - rank_col]
													- (sweep_a_y[k] + sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact - rank_col]
													+ sweep_a_y[k]*Arr[(k - 1)*local_sizes[1] + sw_tact - rank_col]
													+ (sweep_c_y[k] - sweep_a_y[k] - sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact - rank_col];

				buf_x[sw_tact - rank_col] = sweep_b_y[0]/(sweep_c_y[0] - sweep_a_y[0]*tmp_x);
				buf_y[sw_tact - rank_col] = (sweep_f_y[0] + sweep_a_y[0]*tmp_y)/(sweep_c_y[0] - sweep_a_y[0]*tmp_x);
			}
			else
			{
				sweep_f_y[0] = sweep_b_y[0]*Arr[local_sizes[1] + sw_tact - rank_col]
							- (sweep_a_y[0] + sweep_b_y[0])*Arr[sw_tact - rank_col]
							+ sweep_a_y[0]*edge_t[sw_tact - rank_col]
							+ (sweep_c_y[0] - sweep_a_y[0] - sweep_b_y[0])*Arr[sw_tact - rank_col];
				for (k = 1; k < local_sizes[0] - 1; k++) sweep_f_y[k] = sweep_b_y[k]*Arr[(k + 1)*local_sizes[1] + sw_tact - rank_col]
													- (sweep_a_y[k] + sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact - rank_col]
													+ sweep_a_y[k]*Arr[(k - 1)*local_sizes[1] + sw_tact - rank_col]
													+ (sweep_c_y[k] - sweep_a_y[k] - sweep_b_y[k])*Arr[k*local_sizes[1] + sw_tact - rank_col];
				sweep_f_y[local_sizes[0] - 1] = sweep_b_y[local_sizes[0] - 1]*edge_b[sw_tact - rank_col]
									- (sweep_a_y[local_sizes[0] - 1] + sweep_b_y[local_sizes[0] - 1])*Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col]
									+ sweep_a_y[local_sizes[0] - 1]*Arr[(local_sizes[0] - 2)*local_sizes[1] + sw_tact - rank_col]
									+ (sweep_c_y[local_sizes[0] - 1] - sweep_a_y[local_sizes[0] - 1] - sweep_b_y[local_sizes[0] - 1])*Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col];

				buf_x[sw_tact - rank_col] = sweep_b_y[0]/(sweep_c_y[0] - sweep_a_y[0]*tmp_x);
				buf_y[sw_tact - rank_col] = (sweep_f_y[0] + sweep_a_y[0]*tmp_y)/(sweep_c_y[0] - sweep_a_y[0]*tmp_x);				
			}
		}

		if (flag2)
		{
			MPI_Irecv(&tmp_x, 1, MPI_DOUBLE_COMPLEX, rank_col - 1, 17, comm_col, &req_x);
			MPI_Irecv(&tmp_y, 1, MPI_DOUBLE_COMPLEX, rank_col - 1, 18, comm_col, &req_y);
		}

		if (flag1)
		{
			for (j = 1; j < local_sizes[0] - 1; j++)
			{
				buf_x[j*local_sizes[1] + sw_tact - rank_col] = sweep_b_y[j]/(sweep_c_y[j] - sweep_a_y[j]*buf_x[(j - 1)*local_sizes[1] + sw_tact - rank_col]);
				buf_y[j*local_sizes[1] + sw_tact - rank_col] = (sweep_f_y[j] + sweep_a_y[j]*buf_y[(j - 1)*local_sizes[1] + sw_tact - rank_col])/(sweep_c_y[j] - sweep_a_y[j]*buf_x[(j - 1)*local_sizes[1] + sw_tact - rank_col]);
			}

			if (rank_col < psizes[0] - 1)
			{
				buf_x[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col] = sweep_b_y[local_sizes[0] - 1]/(sweep_c_y[local_sizes[0] - 1] - sweep_a_y[local_sizes[0] - 1]*buf_x[(local_sizes[0] - 2)*local_sizes[1] + sw_tact - rank_col]);
				buf_y[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col] = (sweep_f_y[local_sizes[0] - 1] + sweep_a_y[local_sizes[0] - 1]*buf_y[(local_sizes[0] - 2)*local_sizes[1] + sw_tact - rank_col])/(sweep_c_y[local_sizes[0] - 1] - sweep_a_y[local_sizes[0] - 1]*buf_x[(local_sizes[0] - 2)*local_sizes[1] + sw_tact - rank_col]);
			}

			if (rank_col < psizes[0] - 1)
			{
				MPI_Send(&(buf_x[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col]), 1, MPI_DOUBLE_COMPLEX, rank_col + 1, 17, comm_col);
				MPI_Send(&(buf_y[(local_sizes[0] - 1)*local_sizes[1] + sw_tact - rank_col]), 1, MPI_DOUBLE_COMPLEX, rank_col + 1, 18, comm_col);
			}
		}

		if (flag2)
		{
			MPI_Wait(&req_x, &st);
			MPI_Wait(&req_y, &st);
		}
	}

	for (int sw_tact = 0; sw_tact < local_sizes[1] + psizes[0] - 1; sw_tact++)
	{
		bool flag1 = ((rank_col >= psizes[0] - 1 - sw_tact) && (rank_col <= local_sizes[1] + psizes[0] - 2 - sw_tact));
		bool flag2 = ((rank_col < psizes[0] - 1) && (rank_col >= psizes[0] - 2 - sw_tact) && (rank_col <= local_sizes[1] + psizes[0] - 3 - sw_tact));

		if (flag1)
		{
			if (rank_col == psizes[0] - 1)
			{
				de = (Arr[(local_sizes[0] - 2)*local_sizes[1] + sw_tact] - Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact]*(1. - _COMPLEX_I*2.*sqr(dy[local_sizes[0] - 1])/z_step))*ga;
				Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact] = (ga*buf_y[(local_sizes[0] - 2)*local_sizes[1] + sw_tact] + de)/(1. - ga*buf_x[(local_sizes[0] - 2)*local_sizes[1] + sw_tact]);
			}
			else
				Arr[(local_sizes[0] - 1)*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1] = tmp_u*buf_x[(local_sizes[0] - 1)*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1] + buf_y[(local_sizes[0] - 1)*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1];
		}

		if (flag2)
			MPI_Irecv(&tmp_u, 1, MPI_DOUBLE_COMPLEX, rank_col + 1, 19, comm_col, &req_u);

		if (flag1)
		{
			for (j = local_sizes[0] - 2; j >= 0; j--)
				Arr[j*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1] = Arr[(j + 1)*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1]*buf_x[j*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1] + buf_y[j*local_sizes[1] + sw_tact + rank_col - psizes[0] + 1];
			
			if (rank_col > 0)
				MPI_Send(&(Arr[sw_tact + rank_col - psizes[0] + 1]), 1, MPI_DOUBLE_COMPLEX, rank_col - 1, 19, comm_col);
		}

		if (flag2)
			MPI_Wait(&req_u, &st);
	}
}

void Kerr(complex<double> *arr, int n_x, int n_y, double z_step, double *max_pole)
{
	int k;
	double tmp;
	
	for (k = 0; k < n_y*n_x; k++)
	{
		tmp = norm(arr[k]);
		if (tmp > max_pole[0]) max_pole[0] = tmp;
		arr[k] *= exp(-_COMPLEX_I*kerrR*z_step/2.*tmp);
	}
}
