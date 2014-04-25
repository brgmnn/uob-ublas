#include <stdio.h>
#include <stdlib.h>

#include <spflr/simple-profiler.h>
#include <ublas.h>
#include <fann/floatfann.h>

sp_profile_t* tp;

int main() {
	tp = sp_create_profile();

	ublas_settings settings;
	settings.cores = 16;
	settings.library = UBL_AUTO;
	ublas_init(&settings);

	printf("uBLAS Accuracy Tests\n");


	int msize = 512;
	int mul = 1;
	int step = 2;
	int avgsize = 10;

	int bc_cblas = 0;
	int bc_plasma = 0;
	float *da, *db, *dc, t_atlas, t_cublas, t_plasma, t_mkl, t_ublas;
	da = malloc(mul*mul*msize*msize*sizeof(float));
	db = malloc(mul*mul*msize*msize*sizeof(float));
	dc = malloc(mul*mul*msize*msize*sizeof(float));

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE);
	ublas_matrix *b = ublas_new_matrix(2, 2, db, SINGLE);
	ublas_matrix *c = ublas_new_matrix(2, 2, dc, SINGLE);

	// for (int x=0; x<msize-2; x++) {
	// 	for (int y=0; y<msize-2; y++) {

	FILE *f = fopen("../atlas-mkl.data", "w");
	fprintf(f, "%d 2 2\n\n", msize/step);

	int nn_correct = 0;
	int trials = 0;

	fann_type *calc_out;
	fann_type input[2];

//	struct fann *ann = fann_create_from_file("../atlas-plasma-float.net");

	int x, y, m;
	for (x=2, y=2; x<=msize, y<=msize; x+=step, y+=step) {
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

#if defined(WITH_ATLAS)
        // Test CLBAS
    	settings.library = UBL_ATLAS;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("atlas = %fs, ", t_atlas = (sp_stop_timer(tp)/avgsize));
#endif

#if defined(WITH_CUBLAS)
		// Test CUBLAS
		settings.library = UBL_CUBLAS;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("cublas = %fs, ", t_cublas = (sp_stop_timer(tp)/avgsize));		
#endif

#if defined(WITH_PLASMA)
        // Test PLASMA
		settings.library = UBL_PLASMA;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("plasma = %fs, ", t_plasma = (sp_stop_timer(tp)/avgsize));
#endif

#if defined(WITH_MKL)
        // Test MKL
        settings.library = UBL_MKL;
        ublas_gemm(a, b, c, 1.0, 0.0);
        sp_start_timer(tp);
        for (m=0; m<avgsize; m++)
            ublas_gemm(a, b, c, 1.0, 0.0);
        printf("mkl = %fs, ", t_mkl = (sp_stop_timer(tp)/avgsize));
#endif

		settings.library = UBL_AUTO;
        ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("ublas = %fs", t_ublas = (sp_stop_timer(tp)/avgsize));

		fprintf(stderr, "%f,%f,%f\n", t_atlas, t_mkl, t_ublas);
		// fprintf(stderr, "%f,%f,%f,%f\n", t_atlas, t_gotoblas, t_mkl, t_ublas);

        printf("\n");

		// calc_out = fann_run(ann, input);

		// if (t_atlas < t_plasma && calc_out[0] > calc_out[1])
		// 	nn_correct++;
		// else if (t_atlas > t_plasma && calc_out[0] < calc_out[1])
		// 	nn_correct++;

		trials++;
		// if (t_atlas < t_plasma) {
		// 	bc_cblas++;
        // } else {
		// 	bc_plasma++;
		// }


		fprintf(f, "%d %d\n", mul*x, mul*y);
		if (fmax(t_atlas, t_mkl) == t_mkl)
			fprintf(f, "-1 1\n");
		else
			fprintf(f, "1 -1\n");
		// if (fmin(t_atlas, t_gotoblas) == t_atlas && fmin(t_atlas, t_mkl) == t_atlas)
		// 	fprintf(f, "1 -1 -1\n");
		// else if (fmin(t_gotoblas, t_atlas) == t_gotoblas && fmin(t_gotoblas, t_mkl) == t_gotoblas)
		// 	fprintf(f, "-1 1 -1\n");
		// else
		// 	fprintf(f, "-1 -1 1\n");


        // if (x < msize/2)
		// 	fprintf(f, "1 -1\n");
		// else
		// 	fprintf(f, "-1 1\n");
	}

	// printf("cblas best choice:  %f\n", (double)bc_cblas *100/(bc_cblas+bc_plasma));
	// printf("plasma best choice: %f\n", (double)bc_plasma*100/(bc_cblas+bc_plasma));
	printf("Neural Network accuracy: %f\n", 100*((double)nn_correct/trials));

	free(a);
	free(b);
	free(c);

	ublas_free();
}
