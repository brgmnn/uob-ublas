#include "mkl.h"

int ubf_init(void **ctx) {
	*ctx = NULL;
	omp_set_num_threads(16);
	return 0;
}

int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	// printf("using atlas\n");
	ublas_type type = a->type;

	if (type == SINGLE)
		cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
			a->cols, 1.0f, (float*)a->cells, a->cols, (float*)b->cells, b->cols, 0.0f,
			(float*)c->cells, c->cols);
	else if (type == DOUBLE)
		cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
			a->cols, 1.0, (double*)a->cells, a->cols, (double*)b->cells, b->cols, 0.0,
			(double*)c->cells, c->cols);

	return 0;
}

int ubf_free(void *ctx) {
	return 0;
}