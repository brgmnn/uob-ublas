#ifndef UBLAS_LIB_MKL_H
#define UBLAS_LIB_MKL_H

#include <stdio.h>
#include <stdlib.h>
#include <ublas_types.h>

#if defined(BUILD_DRIVER)
#include <mkl_cblas.h>
#include <mkl.h>
#include <omp.h>
#endif

int ubf_init(void **ctx);
int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);
int ubf_free(void *ctx);

#endif
