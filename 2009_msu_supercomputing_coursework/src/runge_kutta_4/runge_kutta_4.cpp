#include "runge_kutta_4.h"

#define N(x,y) ((y)*s_mpi_data->local_nx + (x))
#define Ne(x,y) (((y) + 1)*(s_mpi_data->local_nx + 2) + (x) + 1)

/**
* @todo	Write doccomment
*/
namespace rk4
{
	/**
	* @todo	Write doccomment
	*/
	std::complex<double> do_step_f(std::complex<double> *local_field_extended, int i, int j,
								   sns::mpi::struct_mpi_data *s_mpi_data, struct param_struct *p_parameters)
	{
		std::complex<double> one_div_two_i(0.0, -0.5);
		std::complex<double> diffraction_alpha(s_mpi_data->n*s_mpi_data->n/p_parameters->l/p_parameters->l/4, 0.0);
		
		std::complex<double> ret = 0;
		
		ret  = local_field_extended[Ne(i+1,j)] + local_field_extended[Ne(i-1,j)] - 2.0*local_field_extended[Ne(i,j)];
		ret += local_field_extended[Ne(i,j+1)] + local_field_extended[Ne(i,j-1)] - 2.0*local_field_extended[Ne(i,j)];
		
		return one_div_two_i*( diffraction_alpha*ret + p_parameters->R*norm(local_field_extended[Ne(i,j)])*local_field_extended[Ne(i,j)] );
	}
	
	/**
	* @todo	Write doccomment
	*/
	void do_sendrecieve(std::complex<double> *local_field_extended, sns::mpi::struct_mpi_data *s_mpi_data, std::complex<double> *tmp)
	{
		// Calculate top/right/bottom/left processes ranks
		int mpi_rank_x = s_mpi_data->mpi_rank%s_mpi_data->npx;
		int mpi_rank_y = s_mpi_data->mpi_rank/s_mpi_data->npx;
		
		int mpi_rank_top = mpi_rank_y > 0 ? s_mpi_data->npx*(mpi_rank_y - 1) + mpi_rank_x : -1;
		int mpi_rank_right = mpi_rank_x < s_mpi_data->npx - 1 ? s_mpi_data->npx*mpi_rank_y + mpi_rank_x + 1 : -1;
		int mpi_rank_bottom = mpi_rank_y < s_mpi_data->npy - 1 ? s_mpi_data->npx*(mpi_rank_y + 1) + mpi_rank_x : -1;
		int mpi_rank_left = mpi_rank_x > 0 ? s_mpi_data->npx*mpi_rank_y + mpi_rank_x - 1 : -1;
		
		MPI_Status status;
		int sendtag = 7, recvtag = sendtag;
		
		int i = 0;

		// Sendreceive to top
		if( mpi_rank_top >= 0 ) {
			if( MPI_Sendrecv(local_field_extended + Ne(0,0), s_mpi_data->local_nx, MPI_DOUBLE_COMPLEX, mpi_rank_top, sendtag,
				         local_field_extended + Ne(0,-1), s_mpi_data->local_nx, MPI_DOUBLE_COMPLEX, mpi_rank_top, recvtag,
				         s_mpi_data->mpi_comm, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
			}
		}
		
		// Sendreceive to bottom
		if( mpi_rank_bottom >= 0 ) {
			if( MPI_Sendrecv(local_field_extended + Ne(0,s_mpi_data->local_ny-1), s_mpi_data->local_nx, MPI_DOUBLE_COMPLEX, mpi_rank_bottom, sendtag,
				         local_field_extended + Ne(0,s_mpi_data->local_ny), s_mpi_data->local_nx, MPI_DOUBLE_COMPLEX, mpi_rank_bottom, recvtag,
				         s_mpi_data->mpi_comm, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
			}
		}
		
		// Sendreceive to right
		if( mpi_rank_right >= 0 ) {
			// Copy right column to temporary continuous array
			for( i = 0; i < s_mpi_data->local_ny; i++ )
			{
				tmp[i] = local_field_extended[Ne(s_mpi_data->local_nx-1,i)];
			}
			
			// Sendreceive it
			if( MPI_Sendrecv_replace(tmp, s_mpi_data->local_ny, MPI_DOUBLE_COMPLEX, mpi_rank_right, sendtag, mpi_rank_right, recvtag,
				         s_mpi_data->mpi_comm, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
			}
			
			// Copy received data to right border
			for( i = 0; i < s_mpi_data->local_ny; i++ )
			{
				local_field_extended[Ne(s_mpi_data->local_nx,i)] = tmp[i];
			}
		}
		
		// Sendreceive to left
		if( mpi_rank_left >= 0 ) {
			// Copy left column to temporary continuous array
			for( i = 0; i < s_mpi_data->local_ny; i++ )
			{
				tmp[s_mpi_data->local_ny+i] = local_field_extended[Ne(0,i)];
			}
			
			// Sendreceive it
			if( MPI_Sendrecv_replace(tmp + s_mpi_data->local_ny, s_mpi_data->local_ny, MPI_DOUBLE_COMPLEX, mpi_rank_left, sendtag, mpi_rank_left, recvtag,
				         s_mpi_data->mpi_comm, &status) != MPI_SUCCESS ) {
				MPI_Abort(s_mpi_data->mpi_comm, MPI_ERR_OTHER);
			}
			
			// Copy received data to left border
			for( i = 0; i < s_mpi_data->local_ny; i++ )
			{
				local_field_extended[Ne(-1,i)] = tmp[s_mpi_data->local_ny+i];
			}
		}
	}
	
	/**
	* @todo	Write doccomment
	*/
	int do_step(std::complex<double> *local_field, sns::mpi::struct_mpi_data *s_mpi_data,
	            struct param_struct *p_parameters, std::complex<double> **temp_arrays)
	{
		int i, j;
		
		// Links to temporary arrays
		std::complex<double> *temp0 = temp_arrays[0];
		std::complex<double> *temp1 = temp_arrays[1];
		std::complex<double> *temp2 = temp_arrays[2];
		std::complex<double> *temp3 = temp_arrays[3];
		std::complex<double> *temp4 = temp_arrays[4];
		
		// This method requires "rk4::temp_arrays_count_required" temporary array
		for( i = 0; i < rk4::temp_arrays_count_required; i++ ) {
			if( !temp_arrays[i] ) {
				printf("ERROR: Runge-Kutta 4th order method requires %d temporary array.\n", rk4::temp_arrays_count_required);
				return 1;
			}
		}

		// Copy current field to extended temp0
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				temp0[Ne(i,j)] = local_field[N(i,j)];
			}
		}
		
		do_sendrecieve(temp0, s_mpi_data, temp4);
		
		// k1
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				temp1[Ne(i,j)] = do_step_f(temp0, i, j, s_mpi_data, p_parameters);
			}
		}
		
		do_sendrecieve(temp1, s_mpi_data, temp4);
		
		// k2
		for( j = -1; j <= s_mpi_data->local_ny; j++ )
		{
			for( i = -1; i <= s_mpi_data->local_nx; i++ )
			{
				temp0[Ne(i,j)] = temp0[Ne(i,j)] + temp1[Ne(i,j)]*p_parameters->dz/2.0;
			}
		}
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				temp2[Ne(i,j)] = do_step_f(temp0, i, j, s_mpi_data, p_parameters);
			}
		}
		
		do_sendrecieve(temp2, s_mpi_data, temp4);
		
		// k3
		for( j = -1; j <= s_mpi_data->local_ny; j++ )
		{
			for( i = -1; i <= s_mpi_data->local_nx; i++ )
			{
				temp0[Ne(i,j)] = (temp0[Ne(i,j)] - temp1[Ne(i,j)]*p_parameters->dz/2.0) 
				                + temp2[Ne(i,j)]*p_parameters->dz/2.0;
			}
		}
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				temp3[Ne(i,j)] = do_step_f(temp0, i, j, s_mpi_data, p_parameters);
			}
		}
		
		do_sendrecieve(temp3, s_mpi_data, temp4);
		
		// k4
		for( j = -1; j <= s_mpi_data->local_ny; j++ )
		{
			for( i = -1; i <= s_mpi_data->local_nx; i++ )
			{
				temp0[Ne(i,j)] = (temp0[Ne(i,j)] - temp2[Ne(i,j)]*p_parameters->dz/2.0)
				                + temp3[Ne(i,j)]*p_parameters->dz;
			}
		}
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				temp4[Ne(i,j)] = do_step_f(temp0, i, j, s_mpi_data, p_parameters);
			}
		}
		
		// Make field after step
		for( j = 0; j < s_mpi_data->local_ny; j++ )
		{
			for( i = 0; i < s_mpi_data->local_nx; i++ )
			{
				local_field[N(i,j)] += (temp1[Ne(i,j)] + 2.0*temp2[Ne(i,j)] + 2.0*temp3[Ne(i,j)] + temp4[Ne(i,j)])*p_parameters->dz/6.0;
			}
		}
		
		return 0;
	}
}
