#ifndef UBLAS_LIB_PLASMA_H
#define UBLAS_LIB_PLASMA_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <pthread.h>
#include <plasma.h>
#include <cblas.h>
#include <lapacke.h>
#include <core_blas.h>

#include <ublas_types.h>

int ubf_init(void **ctx);
int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);
int ubf_free(void *ctx);

#endif