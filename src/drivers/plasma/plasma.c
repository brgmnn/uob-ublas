#include "plasma.h"
// int _ubl_plasma_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
// 	// printf("using plasma\n");
// 	ublas_type type = a->type;
// 	int info;
	
// 	// info = PLASMA_Alloc_Workspace_dgesv_incpiv(N, &L, &IPIV);
// 	// info = PLASMA_dgesv_incpiv(N, NRHS, A2, LDA, L, IPIV, B2, LDB);
	
// 	return 0;
// }


int ubf_init(void **ctx) {
	*ctx = NULL;
	PLASMA_Init(16);
	return 0;
}

int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	ublas_type type = a->type;

	if (type == SINGLE) {
		// cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
		// 	a->cols, 1.0f, (float*)a->cells, a->cols, (float*)b->cells, b->cols, 0.0f,
		// 	(float*)c->cells, c->cols);
		PLASMA_sgemm(PlasmaNoTrans, PlasmaNoTrans, c->rows, c->cols, a->cols,
			1.0f, (float*)a->cells, a->rows, (float*)b->cells, b->rows, 0.0f,
			(float*)c->cells, c->rows);
	} else if (type == DOUBLE) {
		// cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
		// 	a->cols, 1.0, (double*)a->cells, a->cols, (double*)b->cells, b->cols, 0.0,
		// 	(double*)c->cells, c->cols);
		PLASMA_dgemm(PlasmaNoTrans, PlasmaNoTrans, c->rows, c->cols, a->cols,
			1.0, (double*)a->cells, a->rows, (double*)b->cells, b->rows, 0.0,
			(double*)c->cells, c->rows);
	}

	return 0;
}

int ubf_free(void *context) {
	PLASMA_Finalize();
	return 0;
}