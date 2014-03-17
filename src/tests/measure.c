#include <stdio.h>
#include <stdlib.h>

#include <spflr/simple-profiler.h>
#include <ublas.h>

sp_profile_t* tp;

int main() {
	tp = sp_create_profile();

	ublas_settings settings;
	settings.cores = 4;
	settings.library = UBL_PLASMA;
	ublas_init(&settings);

	printf("uBLAS Accuracy Tests\n");


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


	FILE *f = fopen("../atlas-plasma.data", "w");

	// for (int x=0; x<msize-2; x++) {
	// 	for (int y=0; y<msize-2; y++) {

	fprintf(f, "%d 2 2\n", msize/4);

	int x, y, m;
	for (x=1, y=1; x<=msize, y<=msize; x+=4, y+=4) {
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
			// fprintf(f, "%f %f\n", 2*((double)x/msize)-1, 2*((double)y/msize)-1);
			fprintf(f, "%d %d\n", x, y);

			if (t_cblas < t_plasma) {
				bc_cblas++;
				fprintf(f, "1 -1\n");
			} else {
				bc_plasma++;
				fprintf(f, "-1 1\n");
			}

			// if (x < msize/2)

    // 	fprintf(f, "1 -1\n");
			// else
			// 	fprintf(f, "-1 1\n");
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
