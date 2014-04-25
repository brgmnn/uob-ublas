#ifndef UBLAS_TESTS_H
#define UBLAS_TESTS_H

#include "ublas_types.h"
#include "matrix.h"

int ublas_check_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c,
	double alpha, double beta, ublas_matrix *benchmark);

#endif