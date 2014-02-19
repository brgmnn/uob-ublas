#include "ublas.h"

ublas_settings *_ub_settings;

void ublas_init(ublas_settings *settings) {
	_ub_settings = settings;
	#ifdef WITH_PLASMA
		PLASMA_Init(settings->cores);
	#endif
}

void ublas_free() {
	#ifdef WITH_PLASMA
		PLASMA_Finalize();
	#endif
}

ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type) {
	ublas_matrix *mat = malloc(sizeof(ublas_matrix));
	mat->rows = rows;
	mat->cols = cols;
	mat->type = type;
	mat->cells = data;
	return mat;
}

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	if (a->cols != b->rows && a->rows != c->rows && b->cols != c->cols)
		return (int)E_MISSMATCH_DIMENSIONS;
	if (a->type != b->type && b->type != c->type)
		return (int)E_MISSMATCH_TYPES;
	
	int info;

	if (_ub_settings->library == UBL_AUTO) {
		// if (1)
		// 	return _ubl_plasma_gemm(a, b, c, alpha, beta);
		// else
		// 	return _ubl_cblas_gemm(a, b, c, alpha, beta);
	}

#if defined(WITH_ATLAS)
	else if (_ub_settings->library == UBL_CBLAS) {
		return _ubl_cblas_gemm(a, b, c, alpha, beta);
	}
#endif
#if defined(WITH_PLASMA)
	else if (_ub_settings->library == UBL_PLASMA) {
		return _ubl_plasma_gemm(a, b, c, alpha, beta);
	}
#endif

	return 0;
}


// calculate the dot product of a matrix
double ublas_ddot( int n, double *x, int iix, double *y, int iiy ) {
	// #ifdef WITH_ATLAS
	// 	return cblas_ddot( n, x, iix, y, iiy );
	// #endif
	return (double)(n+*x+iix+*y+iiy);
}
