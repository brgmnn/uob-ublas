#include "cublas.h"

int ubf_init(void **ctx) {
	libcontext *context = malloc(sizeof(libcontext));
	cublasCreate(&(context->handle));
	*ctx = (void*)context;
	return 0;
}

int ubf_gemm(void *ctx, ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	// printf("using atlas\n");
	libcontext *context = (libcontext*)ctx;
	ublas_type type = a->type;

	if (type == SINGLE) {
		float *dev_a = NULL, *dev_b = NULL, *dev_c = NULL;
		cudaMalloc((void**)&dev_a, a->rows*a->cols*sizeof(float));
		cudaMalloc((void**)&dev_b, b->rows*b->cols*sizeof(float));
		cudaMalloc((void**)&dev_c, c->rows*c->cols*sizeof(float));

		cublasSetMatrix(a->rows, a->cols, sizeof(float), (float*)a->cells, a->cols, dev_a, a->cols);
		cublasSetMatrix(b->rows, b->cols, sizeof(float), (float*)b->cells, b->cols, dev_b, b->cols);
		cublasSetMatrix(c->rows, c->cols, sizeof(float), (float*)c->cells, c->cols, dev_c, c->cols);
	
		float f_alpha = (float)alpha, f_beta = (float)beta;
		cublasSgemm(context->handle, CUBLAS_OP_N, CUBLAS_OP_N, a->rows, b->cols,
			a->cols, &f_alpha, dev_a, a->cols, dev_b, b->cols, &f_beta,
			dev_c, c->cols);
		cudaDeviceSynchronize();

		cublasGetMatrix(c->rows, c->cols, sizeof(float), dev_c, c->cols, (float*)c->cells, c->cols);
		
		cudaFree(dev_a);
		cudaFree(dev_b);
		cudaFree(dev_c);
	}
		// cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
		// 	a->cols, 1.0f, (float*)a->cells, a->cols, (float*)b->cells, b->cols, 0.0f,
		// 	(float*)c->cells, c->cols);
	else if (type == DOUBLE) {

	}
		// cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, c->rows, c->cols,
		// 	a->cols, 1.0, (double*)a->cells, a->cols, (double*)b->cells, b->cols, 0.0,
		// 	(double*)c->cells, c->cols);

	return 0;
}

int ubf_free(void *ctx) {
	libcontext *context = (libcontext*)ctx;
	cublasDestroy(context->handle);
}
