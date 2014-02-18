#include <stdio.h>

#include <ublas.h>

int main() {
	printf("uBLAS Accuracy Tests\n");
	ublas_settings settings;
	settings.cores = 2;
	ublas_init(&settings);

	double da[2][2];
	double db[2][2];
	double dc[2][2];

	ublas_free();
	return 0;
}