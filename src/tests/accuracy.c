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
	int step = 8;
	int avgsize = 10;
	int start = 32;

	int bc_cblas = 0;
	int bc_plasma = 0;
	float *da, *db, *dc, t_ublas;
	float libtime[UBL_COUNT-1];
	da = malloc(mul*mul*msize*msize*sizeof(float));
	db = malloc(mul*mul*msize*msize*sizeof(float));
	dc = malloc(mul*mul*msize*msize*sizeof(float));

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE);
	ublas_matrix *b = ublas_new_matrix(2, 2, db, SINGLE);
	ublas_matrix *c = ublas_new_matrix(2, 2, dc, SINGLE);

	// for (int x=0; x<msize-2; x++) {
	// 	for (int y=0; y<msize-2; y++) {

	FILE *f = fopen("../training.data", "w");
	fprintf(f, "%d 2 4\n\n", msize/step);

	fprintf(f, "x,y,");
#if defined(WITH_ATLAS)
	fprintf(f, "atlas,");
#endif
#if defined(WITH_CUBLAS)
	fprintf(f, "cublas,");
#endif
#if defined(WITH_MKL)
	fprintf(f, "mkl,");
#endif
#if defined(WITH_PLASMA)
	fprintf(f, "plasma,");
#endif
	fprintf(f, "ublas,");

	int nn_correct = 0;
	int trials = 0;

	fann_type *calc_out;
	fann_type input[2];

//	struct fann *ann = fann_create_from_file("../atlas-plasma-float.net");
	fprintf(f, "x,y,");

	int x, y, m;
	for (x=start, y=start; x<=msize, y<=msize; x+=step, y+=step) {
	// for (x=start, y=16; x<=msize && y<=msize; x+=step) {
	 // for (x=16, y=start; x<=msize, y<=msize; y+=step) {
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
		printf("atlas = %fs, ", libtime[UBL_ATLAS-1] = (sp_stop_timer(tp)/avgsize));
#endif

#if defined(WITH_CUBLAS)
		// Test CUBLAS
		settings.library = UBL_CUBLAS;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("cublas = %fs, ", libtime[UBL_CUBLAS-1] = (sp_stop_timer(tp)/avgsize));		
#endif

#if defined(WITH_MKL)
        // Test MKL
        settings.library = UBL_MKL;
        ublas_gemm(a, b, c, 1.0, 0.0);
        sp_start_timer(tp);
        for (m=0; m<avgsize; m++)
            ublas_gemm(a, b, c, 1.0, 0.0);
        printf("mkl = %fs, ", libtime[UBL_MKL-1] = (sp_stop_timer(tp)/avgsize));
#endif

#if defined(WITH_PLASMA)
        // Test PLASMA
		settings.library = UBL_PLASMA;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("plasma = %fs, ", libtime[UBL_PLASMA-1] = (sp_stop_timer(tp)/avgsize));
#endif

		settings.library = UBL_AUTO;
        ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (m=0; m<avgsize; m++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		printf("ublas = %fs", t_ublas = (sp_stop_timer(tp)/avgsize));

		printf(" ---> %d", ublas_ffastest(libtime));

		// print results to a csv file
		fprintf(stderr, "%d,%d,", x, y);
		for (int i=0; i<UBL_COUNT-1; i++)
			fprintf(stderr, "%f,", libtime[i]);
		fprintf(stderr, "%f\n", t_ublas);
        // fprintf(stderr, "    %d\n", ublas_ffastest(libtime));

        printf("\n");



  //       printf("    ");
		// for (int i=0; i<UBL_COUNT-1; i++)
		// 	printf("%.5f, ", libtime[i]);
  //       printf(" --- ublas = %.5f\n", t_ublas);

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
		ublas_library bestlib = ublas_ffastest(libtime);
		for (int i=1; i<UBL_COUNT; i++) {
			if (i == bestlib)
				fprintf(f, "1 ");
			else
				fprintf(f, "-1 ");
		}
		fprintf(f, "\n");

		// if (fmax(t_atlas, t_mkl) == t_mkl)
		// 	fprintf(f, "-1 1\n");
		// else
		// 	fprintf(f, "1 -1\n");
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
