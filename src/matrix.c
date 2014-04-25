#include "matrix.h"

ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type) {
	ublas_matrix *mat = malloc(sizeof(ublas_matrix));
	mat->rows = rows;
	mat->cols = cols;
	mat->type = type;
	mat->cells = data;
	return mat;
}

int ublas_matrix_equal(ublas_matrix *a, ublas_matrix *b) {
	if (a->rows != b->rows)
		return -1;
	if (a->cols != b->cols)
		return -2;
	if (a->type != b->type)
		return -3;

	if (a->type == SINGLE) {
		float *da = (float*)a->cells, *db = (float*)b->cells;
		for (int i=0; i<a->rows; i++)
			for (int j=0; j<a->cols; j++)
				if (da[i*a->cols+j] != db[i*a->cols+j])
					return i*a->cols+j+1;
	} else if (a->type == DOUBLE) {
		double *da = (double*)a->cells, *db = (double*)b->cells;
		for (int i=0; i<a->rows; i++)
			for (int j=0; j<a->cols; j++)
				if (da[i*a->cols+j] != db[i*a->cols+j])
					return i*a->cols+j+1;
	}
	return 0;
}

void ublas_matrix_print(ublas_matrix *a) {
	if (a->type == DOUBLE) {
		double *da = (double*)a->cells;
		for (int i=0; i<a->rows; i++) {
			for (int j=0; j<a->cols; j++)
				printf("%f ", da[i*a->cols+j]);
			printf("\n");
		}
	} else {
		float *da = (float*)a->cells;
		for (int i=0; i<a->rows; i++) {
			for (int j=0; j<a->cols; j++)
				printf("%f ", da[i*a->cols+j]);
			printf("\n");
		}
	}
}