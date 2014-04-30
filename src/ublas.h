#ifndef UBLAS_H
#define UBLAS_H

#include <stdlib.h>
#include <float.h>

#if !defined(WITH_ATLAS) && !defined(WITH_CUBLAS) && !defined(WITH_MKL) && !defined(WITH_PLASMA)
#pragma GCC error "No blas libraries!"
#endif

#include <ublas_types.h>
#include <fann/floatfann.h>

#include "matrix.h"
#include "driver.h"
#include "tests.h"

void ublas_init(ublas_settings *settings);
void ublas_free();

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);

ublas_library ublas_ffastest(float *times);
ublas_library ublas_dfastest(double *times);

#endif
