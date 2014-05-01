#include <stdio.h>
#include <stdlib.h>

#include <spflr/simple-profiler.h>
#include <ublas.h>
#include <fann/floatfann.h>

sp_profile_t* tp;

int main() {
	tp = sp_create_profile();

	ublas_settings settings;
	settings.cores = 8;
	settings.library = UBL_AUTO;
	ublas_init(&settings);

	printf("uBLAS Accuracy Tests\n");


	int msize = 512;
	int mul = 1;
	int step = 8;
	int avgsize = 10;
	int start = 8;

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
	fprintf(f, "                               \n\n");

	fprintf(stderr, "m,k,n,");
#if defined(WITH_ATLAS)
	fprintf(stderr, "atlas,");
#endif
#if defined(WITH_CUBLAS)
	fprintf(stderr, "cublas,");
#endif
#if defined(WITH_MKL)
	fprintf(stderr, "mkl,");
#endif
#if defined(WITH_PLASMA)
	fprintf(stderr, "plasma,");
#endif
	fprintf(stderr, "ublas\n");

	int nn_correct = 0;
	int trials = 0;

	fann_type *calc_out;
	fann_type input[2];

//	struct fann *ann = fann_create_from_file("../atlas-plasma-float.net");

	int x, y, z, m;
	for (x=start, y=start, z=start; x<=msize, y<=msize, z<=msize; x+=step, y+=step, z+=step) {
	// for (x=start, y=16; x<=msize && y<=msize; x+=step) {
	 // for (x=16, y=start; x<=msize, y<=msize; y+=step) {
		printf("m=%4d n=%4d k=%4d: ", mul*x, mul*z, mul*y);
		fflush(stdout);

		// x = m, y = k, z = n
		a->rows = mul*x;
		a->cols = mul*y;
		b->rows = mul*y;
		b->cols = mul*z;
		c->rows = mul*x;
		c->cols = mul*z;

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
		fprintf(stderr, "%d,%d,%d,", x, y, z);
		for (int i=0; i<UBL_COUNT-1; i++)
			fprintf(stderr, "%f,", libtime[i]);
		fprintf(stderr, "%f\n", t_ublas);
        // fprintf(stderr, "    %d\n", ublas_ffastest(libtime));

        printf("\n");


        // generates the neural network file
		fprintf(f, "%d %d %d\n", mul*x, mul*z, mul*y);
		ublas_library bestlib = ublas_ffastest(libtime);
		for (int i=1; i<UBL_COUNT; i++) {
			if (i == bestlib)
				fprintf(f, "1 ");
			else
				fprintf(f, "-1 ");
		}
		fprintf(f, "\n");
		trials++;
	}

	fseek(f, 0, SEEK_SET);
	fprintf(f, "%d 3 4", trials);
	fclose(f);

	ublas_free();
}
