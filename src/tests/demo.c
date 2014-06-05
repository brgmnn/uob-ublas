#include <stdio.h>
#include <string.h>
#include <ublas.h>
#include <spflr/simple-profiler.h>

sp_profile_t* tp;

int favgsize(unsigned long m, unsigned long k, unsigned long n) {
	unsigned long prod = m*k*n;

	if (prod < 1000000ul)
		return 100; // square 100
	if (prod < 27000000ul)
		return 30;	// square 300
	if (prod < 216000000ul)
		return 4;	// square 600
	return 1;
}

int main() {
	ublas_settings settings;
	settings.cores = 8;
	settings.library = UBL_AUTO;
	ublas_init(&settings);
	tp = sp_create_profile();

	int trial = 1;
	int s, avgsize = 1, m = 1024, n = 1024, k = 1024;
	float in[3] = {0, 0, 0};
	float exectime;
	ublas_library qlib;
	
	// scanf("%d %d %d %d", &avgsize, &m, &k, &n);

	ublas_matrix *a = ublas_new_matrix2(m, k, SINGLE);
	ublas_matrix *b = ublas_new_matrix2(k, n, SINGLE);
	ublas_matrix *c = ublas_new_matrix2(m, n, SINGLE);
	double tm;

	while (1) {
		m = 0; n = 0; k = 0;
		char slib[10];
		scanf("%d %d %d", &m, &k, &n);

		if (m <= 0 || n <= 0 || k <= 0 || !strncmp(slib, "stop", 4))
			break;

		// x = m, y = k, z = n
		a->rows = m;
		a->cols = k;
		b->rows = k;
		b->cols = n;
		c->rows = m;
		c->cols = n;

		avgsize = favgsize(m, k, n);

#if defined(WITH_ATLAS)
		settings.library = UBL_ATLAS;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (s=0; s<avgsize; s++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		tm = (sp_stop_timer(tp)/avgsize);
		printf("atlas = %f\n", tm);
#endif
#if defined(WITH_CUBLAS)
		settings.library = UBL_CUBLAS;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (s=0; s<avgsize; s++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		tm = (sp_stop_timer(tp)/avgsize);
		printf("cublas = %f\n", tm);
#endif
#if defined(WITH_MKL)
		settings.library = UBL_MKL;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (s=0; s<avgsize; s++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		tm = (sp_stop_timer(tp)/avgsize);
		printf("mkl = %f\n", tm);
#endif
#if defined(WITH_PLASMA)
		settings.library = UBL_PLASMA;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (s=0; s<avgsize; s++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		tm = (sp_stop_timer(tp)/avgsize);
		printf("plasma = %f\n", tm);
#endif
		settings.library = UBL_AUTO;
		ublas_gemm(a, b, c, 1.0, 0.0);
		sp_start_timer(tp);
		for (s=0; s<avgsize; s++)
			ublas_gemm(a, b, c, 1.0, 0.0);
		tm = (sp_stop_timer(tp)/avgsize);
		printf("\nublas = %f\n", tm);

		fflush(stdout);
		// fprintf(stderr, "-- trial %4d - %6s - (%3d,%3d,%3d)\n", trial++, slib, m, k, n);
	}

	return 0;
}