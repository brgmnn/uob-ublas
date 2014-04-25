#ifndef UBLAS_MATRIX_H
#define UBLAS_MATRIX_H

#include <ublas_types.h>

ublas_matrix* ublas_new_matrix(int rows, int cols, void *data, ublas_type type);
int  ublas_matrix_equal(ublas_matrix *a, ublas_matrix *b);
void ublas_matrix_print(ublas_matrix *a);

#endif