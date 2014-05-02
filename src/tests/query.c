#include <stdio.h>
#include <ublas.h>

int main() {
	ublas_settings settings;
	settings.cores = 8;
	settings.library = UBL_AUTO;
	ublas_init(&settings);

	int m = 1, n = 1, k = 1;
	ublas_library qlib;

	while (1) {
		m = 0; n = 0; k = 0;
		scanf("%d %d %d", &m, &k, &n);
		
		if (m <= 0 || n <= 0 || k <= 0)
			break;

		float in[3] = {m, n, k};
		qlib = ublas_query_ann(in, 3);

#if defined(WITH_ATLAS)
		if (qlib == UBL_ATLAS)
			printf("atlas\n");
#endif
#if defined(WITH_CUBLAS)
		if (qlib == UBL_CUBLAS)
			printf("cublas\n");
#endif
#if defined(WITH_MKL)
		if (qlib == UBL_MKL)
			printf("mkl\n");
#endif
#if defined(WITH_PLASMA)
		if (qlib == UBL_PLASMA)
			printf("plasma\n");
#endif
		fflush(stdout);
	}

	return 0;
}