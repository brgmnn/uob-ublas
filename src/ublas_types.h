#ifndef UBLAS_TYPES_H
#define UBLAS_TYPES_H

#include <fann/fann.h>
#include "ublas_constants.h"

typedef void (*func)();

typedef enum {
	SINGLE,
	DOUBLE,
} ublas_type;

typedef enum {
	E_MISSMATCH_DIMENSIONS,
	E_MISSMATCH_TYPES
} ublas_error;

/*  An enumeration for specifying the underlying library to use or to auto select the
 *  library used. */
typedef enum {
	UBL_AUTO = 0,

#if defined(WITH_ATLAS)
    UBL_ATLAS,
#endif
#if defined(WITH_CUBLAS)
    UBL_CUBLAS,
#endif
#if defined(WITH_MKL)
    UBL_MKL,
#endif
#if defined(WITH_PLASMA)
	UBL_PLASMA,
#endif

	UBL_COUNT
} ublas_library;

typedef enum {
	UBF_INIT,	// initialisation function
	UBF_FREE,	// cleanup function
	UBF_GEMM,	// matrix matrix multiplication

	UBF_COUNT
} ublas_func;

/*  The main uBLAS handler which holds runtime information such as settings and
 *  parameters for the learning agent. */
typedef struct {
	int cores; // select number of cores.
	ublas_library library; // force a library to use. UBL_AUTO lets ublas choose.

	struct fann *ann; // neural network handle

	// function array.
	func **call;
	void **libctx;

// #if defined(WITH_ATLAS)
// 	func call_atlas;
// #endif
// #if defined(WITH_CUBLAS)
// 	func call_cublas;
// #endif
// #if defined(WITH_GOTOBLAS)
// 	func call_gotoblas;
// #endif
// #if defined(WITH_MKL)
// 	func call_mkl;
// #endif

} ublas_settings;

typedef struct {
	void *cells;
	ublas_type type;
	int rows;
	int cols;
} ublas_matrix;

extern ublas_settings *_ub_settings;

#endif
