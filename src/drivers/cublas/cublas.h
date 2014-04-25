#ifndef UBLAS_LIB_CUBLAS_H
#define UBLAS_LIB_CUBLAS_H

#include <stdio.h>
#include <stdlib.h>
#include <ublas_types.h>

#if defined(BUILD_DRIVER)
#include <cuda_runtime.h>
#include <cublas_v2.h>
#endif

typedef struct {
	cublasHandle_t handle;
} libcontext;

int ubf_init(void **ctx);
int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);
int ubf_free(void *ctx);

#endif
