#include <stdio.h>
#include <stdlib.h>

#include <simple-profiler.h>
#include <ublas.h>

sp_profile_t* tp;

int main() {
	tp = sp_create_profile();
	
	ublas_settings settings;
	settings.cores = 2;
	settings.library = UBL_PLASMA;
	ublas_init(&settings);

	printf("uBLAS Accuracy Tests\n");


	// double da[4] = {1.0, 3.0, 4.0, 2.0};
	// double db[4] = {1.0, 0.0, 0.0, 1.0};
	// double dc[4] = {0.0, 0.0, 0.0, 0.0};

	// ublas_matrix *a = ublas_new_matrix(2, 2, da, DOUBLE);
	// ublas_matrix *b = ublas_new_matrix(2, 2, db, DOUBLE);
	// ublas_matrix *c = ublas_new_matrix(2, 2, dc, DOUBLE);

	// sp_start_timer(tp);
	// ublas_gemm(a, b, c, 1.0, 0.0);
	// printf("time taken was: %f s\n", sp_stop_timer(tp));

	int trials = 100;

	double *a, *b, *c;
	a = malloc(trials*trials*sizeof(double));
	b = malloc(trials*trials*sizeof(double));
	c = malloc(trials*trials*sizeof(double));

	for (int t=0; t<trials; t++) {

	}

	ublas_free();
	return 0;
}