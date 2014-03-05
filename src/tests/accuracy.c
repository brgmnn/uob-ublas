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


	float da[4] = {1.0f, 3.0f, 4.0f, 2.0f};
	float db[4] = {1.0f, 0.0f, 0.0f, 1.0f};
	float dc[4] = {0.0f, 0.0f, 0.0f, 0.0f};

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE);
	ublas_matrix *b = ublas_new_matrix(2, 2, db, SINGLE);
	ublas_matrix *c = ublas_new_matrix(2, 2, dc, SINGLE);

	sp_start_timer(tp);
	ublas_gemm(a, b, c, 1.0, 0.0);
	printf("time taken was: %f s\n", sp_stop_timer(tp));

	for (int i=0; i<4; i++) {
		printf("%f\n", dc[i]);
	}

	ublas_free();
	return 0;
}