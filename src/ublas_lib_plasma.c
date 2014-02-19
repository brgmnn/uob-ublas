#include <ublas_lib_plasma.h>

int _ubl_plasma_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	printf("using plasma\n");
	ublas_type type = a->type;
	int info;
	
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
	
	return 0;
}