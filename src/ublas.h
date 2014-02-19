#ifndef UBLAS_H
#define UBLAS_H

#include <stdlib.h>

#if !defined(WITH_ATLAS) && !defined(WITH_PLASMA)
#pragma GCC error "No blas libraries!"
#endif

#ifdef WITH_ATLAS
#include <ublas_lib_cblas.h>
#endif

#ifdef WITH_PLASMA
#include <ublas_lib_plasma.h>
#endif

#include <ublas_types.h>

extern ublas_settings *_ub_settings;

void ublas_init(ublas_settings *settings);
void ublas_free();
ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type);

double ublas_ddot(int n, double *x, int iix, double *y, int iiy);

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);

#endif