#include <stdio.h>
#include <stdlib.h>

#include <simple-profiler.h>
#include <ublas.h>
#include <floatfann.h>

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
	float *da, *db, *dc, t_cblas, t_plasma, t_ublas;
	da = malloc(mul*mul*msize*msize*sizeof(float));
	db = malloc(mul*mul*msize*msize*sizeof(float));
	dc = malloc(mul*mul*msize*msize*sizeof(float));

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE);
	ublas_matrix *b = ublas_new_matrix(2, 2, db, SINGLE);
	ublas_matrix *c = ublas_new_matrix(2, 2, dc, SINGLE);

	// for (int x=0; x<msize-2; x++) {
	// 	for (int y=0; y<msize-2; y++) {

	// fprintf(f, "%d 2 2\n", msize/4);

	int nn_correct = 0;
	int trials = 0;

	fann_type *calc_out;
	fann_type input[2];

	struct fann *ann = fann_create_from_file("../atlas-plasma-float.net");

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

			input[0] = x;
			input[1] = y;

			settings.library = UBL_CBLAS;

			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("cblas = %fs, ", t_cblas = (sp_stop_timer(tp)/avgsize));

			settings.library = UBL_PLASMA;

			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("plasma = %fs, ", t_plasma = (sp_stop_timer(tp)/avgsize));

			settings.library = UBL_AUTO;

			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("ublas = %fs\n", t_ublas = (sp_stop_timer(tp)/avgsize));

			fprintf(stderr, "%f,%f,%f\n", t_cblas, t_plasma, t_ublas);

			// calc_out = fann_run(ann, input);

			// if (t_cblas < t_plasma && calc_out[0] > calc_out[1])
			// 	nn_correct++;
			// else if (t_cblas > t_plasma && calc_out[0] < calc_out[1])
			// 	nn_correct++;

			trials++;
			// if (t_cblas < t_plasma) {
			// 	bc_cblas++;
			// } else {
			// 	bc_plasma++;
			// }

			// if (x < msize/2)
			// 	fprintf(f, "1 -1\n");
			// else
			// 	fprintf(f, "-1 1\n");
	}
	// 	}
	// }

	// printf("cblas best choice:  %f\n", (double)bc_cblas *100/(bc_cblas+bc_plasma));
	// printf("plasma best choice: %f\n", (double)bc_plasma*100/(bc_cblas+bc_plasma));
	printf("Neural Network accuracy: %f\n", 100*((double)nn_correct/trials));

	free(a);
	free(b);
	free(c);

	ublas_free();
}