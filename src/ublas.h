#ifndef UBLAS_H
#define UBLAS_H

#include <stdlib.h>

#if !defined(WITH_ATLAS) && !defined(WITH_GOTOBLAS) && !defined(WITH_MKL) && !defined(WITH_PLASMA)
#pragma GCC error "No blas libraries!"
#endif

#if defined(WITH_ATLAS)
#include <drivers/atlas.h>
#endif

#if defined(WITH_CUBLAS)
#include <drivers/cublas.h>
#endif

#if defined(WITH_MKL)
#include <drivers/mkl.h>
#endif

#if defined(WITH_PLASMA)
#include <drivers/plasma/plasma.h>
#endif

#if defined(WITH_GOTOBLAS)
#include <drivers/gotoblas/gotoblas.h>
#include <drivers/gotoblas.h>
#endif

#include <lib/tpl.h>
#include <ublas_types.h>
#include <fann/floatfann.h>

#include "driver.h"

void ublas_init(ublas_settings *settings);
void ublas_free();
ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type);

double ublas_ddot(int n, double *x, int iix, double *y, int iiy);

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);

#endif
