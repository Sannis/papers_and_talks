#ifndef RUNGE_KUTTA_4_H
#define RUNGE_KUTTA_4_H

#include <complex>

#include "param_struct.h"
#include "sns_mpi.h"

/**
* @todo	Write doccomment
*/
namespace rk4
{
	// This method requires 5 temporary arrays
	const int temp_arrays_count_required = 5;
	
	/**
	* @todo	Write doccomment
	*/
	std::complex<double> do_step_f(std::complex<double> *local_field_extended, int i, int j,
								   sns::mpi::struct_mpi_data *s_mpi_data, struct param_struct *p_parameters);
	
	/**
	* @todo	Write doccomment
	*/
	void do_sendrecieve(std::complex<double> *local_field_extended, sns::mpi::struct_mpi_data *s_mpi_data, std::complex<double> *tmp);
	
	/**
	* @todo	Write doccomment
	*/
	int do_step(std::complex<double> *local_field, sns::mpi::struct_mpi_data *s_mpi_data,
	            struct param_struct *p_parameters, std::complex<double> **temp_arrays);
}

#endif

