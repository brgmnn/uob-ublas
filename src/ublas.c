#include "ublas.h"

void ublas_init(ublas_settings *settings) {
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
	ublas_type type = a->type;

	if (c->rows*c->cols > 25) {
		// info = PLASMA_Alloc_Workspace_dgesv_incpiv(N, &L, &IPIV);
		// info = PLASMA_dgesv_incpiv(N, NRHS, A2, LDA, L, IPIV, B2, LDB);
		if (type == SINGLE)
			info = PLASMA_sgemm(PlasmaNoTrans, PlasmaNoTrans, c->rows, c->cols, a->cols,
				1.0f, (float*)a->cells, a->cols, (float*)b->cells, b->cols, 0.0f,
				(float*)c->cells, c->cols);
		else if (type == DOUBLE)
			info = PLASMA_dgemm(PlasmaNoTrans, PlasmaNoTrans, c->rows, c->cols, a->cols,
				1.0, (double*)a->cells, a->cols, (double*)b->cells, b->cols, 0.0,
				(double*)c->cells, c->cols);
	} else {
		if (type == SINGLE)
			cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
				a->cols, 1.0f, (float*)a->cells, a->cols, (float*)b->cells, b->cols, 0.0f,
				(float*)c->cells, c->cols);
		else if (type == DOUBLE)
			cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
				a->cols, 1.0, (double*)a->cells, a->cols, (double*)b->cells, b->cols, 0.0,
				(double*)c->cells, c->cols);
	}

	return 0;
}


// calculate the dot product of a matrix
double ublas_ddot( int n, double *x, int iix, double *y, int iiy ) {
	// #ifdef WITH_ATLAS
	// 	return cblas_ddot( n, x, iix, y, iiy );
	// #endif
	return (double)(n+*x+iix+*y+iiy);
}
