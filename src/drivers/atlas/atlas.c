#include <atlas.h>

int ubf_init(void **ctx) {
	*ctx = NULL;
	return 0;
}

int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	ublas_type type = a->type;

	if (type == SINGLE)
		cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
			a->cols, 1.0f, (float*)a->cells, a->rows, (float*)b->cells, b->rows, 0.0f,
			(float*)c->cells, c->rows);
	else if (type == DOUBLE)
		cblas_dgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
			a->cols, 1.0, (double*)a->cells, a->rows, (double*)b->cells, b->rows, 0.0,
			(double*)c->cells, c->rows);

	return 0;
}

int ubf_free(void *context) {
	return 0;
}