#ifndef UBLAS_LIB_CLBAS_H
#define UBLAS_LIB_CBLAS_H

#include <stdio.h>
#include <ublas_types.h>
#include <cblas.h>


int _ubl_cblas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);

#endif