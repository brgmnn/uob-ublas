#include "driver.h"

func* ublas_load_driver(char* path) {
	void *lib = dlopen(path, RTLD_NOW);
	if (!lib) {
		printf("Error loading library: %s\n", dlerror());
		exit(1);
	}

	func *api = malloc(sizeof(func)*UBF_COUNT);

	api[UBF_INIT] = dlsym(lib, "ubf_init");
	if (!api[UBF_INIT]) {
		printf("Error loading symbol: %s\n", dlerror());
		exit(1);
	}

	api[UBF_FREE] = dlsym(lib, "ubf_free");
	if (!api[UBF_FREE]) {
		printf("Error loading symbol: %s\n", dlerror());
		exit(1);
	}

	api[UBF_GEMM] = dlsym(lib, "ubf_gemm");
	if (!api[UBF_GEMM]) {
		printf("Error loading symbol: %s\n", dlerror());
		exit(1);
	}

	return api;
}