#ifndef UBLAS_LIB_ATLAS_H
#define UBLAS_LIB_ATLAS_H

#include <stdio.h>
#include <ublas_types.h>
#include <cblas.h>

int ubf_init(void **ctx);
int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);
int ubf_free(void *ctx);

#endif