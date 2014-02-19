#ifndef UBLAS_TYPES_H
#define UBLAS_TYPES_H

typedef enum {
	SINGLE,
	DOUBLE,
} ublas_type;

typedef enum {
	E_MISSMATCH_DIMENSIONS,
	E_MISSMATCH_TYPES
} ublas_error;

typedef enum {
	UBL_AUTO,
	UBL_CBLAS,
	UBL_PLASMA
} ublas_library;

typedef struct {
	int cores; // select number of cores.
	ublas_library library; // force a library to use. UBL_AUTO lets ublas choose. 
} ublas_settings;

typedef struct {
	void *cells;
	ublas_type type;
	int rows;
	int cols;
} ublas_matrix;

#endif