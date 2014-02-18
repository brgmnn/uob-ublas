#ifndef UBLAS_H
#define UBLAS_H

#include <stdlib.h>

#ifdef WITH_ATLAS
	#include <cblas.h>
#endif

#ifdef WITH_PLASMA
	#include <pthread.h>

	#include <stdlib.h>
	#include <stdio.h>
	#include <string.h>
	#include <math.h>

	#include <plasma.h>
	#include <cblas.h>
	#include <lapacke.h>
	#include <core_blas.h>
#endif

typedef enum {
	SINGLE,
	DOUBLE,
} ublas_type;

typedef enum {
	E_MISSMATCH_DIMENSIONS,
	E_MISSMATCH_TYPES
} ublas_error;

typedef struct {
	int cores;
} ublas_settings;

typedef struct {
	void *cells;
	ublas_type type;
	int rows;
	int cols;
} ublas_matrix;

void ublas_init(ublas_settings *settings);
void ublas_free();
ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type);

double ublas_ddot(int n, double *x, int iix, double *y, int iiy);

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta);

#endif