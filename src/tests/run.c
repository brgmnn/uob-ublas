#include <stdio.h>
#include <string.h>
#include <ublas.h>
#include <spflr/simple-profiler.h>

sp_profile_t* tp;

int main() {
	ublas_settings settings;
	settings.cores = 8;
	settings.library = UBL_AUTO;
	ublas_init(&settings);
	tp = sp_create_profile();

	int m = 1, n = 1, k = 1, avgsize = 1;
	float in[3] = {0, 0, 0};
	float exectime;
	ublas_library qlib;
	
	scanf("%d %d %d %d", &avgsize, &m, &k, &n);

	ublas_matrix *a = ublas_new_matrix2(m, k, SINGLE);
	ublas_matrix *b = ublas_new_matrix2(k, n, SINGLE);
	ublas_matrix *c = ublas_new_matrix2(m, n, SINGLE);
	
	while (1) {
		m = 0; n = 0; k = 0;
		char slib[10];
		scanf("%10s %d %d %d", slib, &m, &k, &n);

		if (m <= 0 || n <= 0 || k <= 0 || !strncmp(slib, "stop", 4))
			break;

		// x = m, y = k, z = n
		a->rows = m;
		a->cols = k;
		b->rows = k;
		b->cols = n;
		c->rows = m;
		c->cols = n;

#if defined(WITH_ATLAS)
		if (!strncmp(slib, "atlas", 5)) {
			settings.library = UBL_ATLAS;
			ublas_gemm(a, b, c, 1.0, 0.0);
			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("%f\n", (sp_stop_timer(tp)/avgsize));
		}
#endif
#if defined(WITH_CUBLAS)
		if (!strncmp(slib, "cublas", 6)) {
			settings.library = UBL_CUBLAS;
			ublas_gemm(a, b, c, 1.0, 0.0);
			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("%f\n", (sp_stop_timer(tp)/avgsize));
		}
#endif
#if defined(WITH_MKL)
		if (!strncmp(slib, "mkl", 3)) {
			settings.library = UBL_MKL;
			ublas_gemm(a, b, c, 1.0, 0.0);
			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("%f\n", (sp_stop_timer(tp)/avgsize));
		}
#endif
#if defined(WITH_PLASMA)
		if (!strncmp(slib, "plasma", 6)) {
			settings.library = UBL_PLASMA;
			ublas_gemm(a, b, c, 1.0, 0.0);
			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("%f\n", (sp_stop_timer(tp)/avgsize));
		}
#endif
		if (!strncmp(slib, "auto", 4)) {
			settings.library = UBL_AUTO;
			ublas_gemm(a, b, c, 1.0, 0.0);
			sp_start_timer(tp);
			for (m=0; m<avgsize; m++)
				ublas_gemm(a, b, c, 1.0, 0.0);
			printf("%f\n", (sp_stop_timer(tp)/avgsize));
		}

		fflush(stdout);
	}

	return 0;
}