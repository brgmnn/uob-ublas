#include <stdio.h>
#include <stdlib.h>

#include <simple-profiler.h>
#include <ublas.h>

sp_profile_t* tp;

int main() {
	tp = sp_create_profile();
	
	ublas_settings settings;
	settings.cores = 4;
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

	int msize = 1000;
	int mul = 1;
	int avgsize = 10;

	int bc_cblas = 0;
	int bc_plasma = 0;
	float *da, *db, *dc, t_cblas, t_plasma;
	da = malloc(mul*mul*msize*msize*sizeof(float));
	db = malloc(mul*mul*msize*msize*sizeof(float));
	dc = malloc(mul*mul*msize*msize*sizeof(float));

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE);
	ublas_matrix *b = ublas_new_matrix(2, 2, db, SINGLE);
	ublas_matrix *c = ublas_new_matrix(2, 2, dc, SINGLE);



	// for (int x=0; x<msize-2; x++) {
	// 	for (int y=0; y<msize-2; y++) {
	int x, y, m;
	for (x=1, y=1; x<=msize, y<=msize; x++, y++) {
			printf("x=%4d y=%4d: ", mul*x, mul*y);
			fflush(stdout);

			a->rows = mul*x;
			a->cols = mul*y;
			b->rows = mul*y;
			b->cols = mul*x;
			c->rows = mul*x;
			c->cols = mul*x;


			settings.library = UBL_CBLAS;

			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("cblas = %fs, ", t_cblas = (sp_stop_timer(tp)/avgsize));

			settings.library = UBL_PLASMA;

			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("plasma = %fs\n", t_plasma = (sp_stop_timer(tp)/avgsize));

			fprintf(stderr, "%f,%f\n", t_cblas, t_plasma);

			if (t_cblas < t_plasma)
				bc_cblas++;
			else
				bc_plasma++;
	}
	// 	}
	// }

	printf("cblas best choice:  %f\n", (double)bc_cblas *100/(bc_cblas+bc_plasma));
	printf("plasma best choice: %f\n", (double)bc_plasma*100/(bc_cblas+bc_plasma));

	free(a);
	free(b);
	free(c);

	ublas_free();
	return 0;
}