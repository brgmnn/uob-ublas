#include <ublas.h>

ublas_settings *_ub_settings;

void ublas_init(ublas_settings *settings) {
	_ub_settings = settings;

	settings->call = malloc(sizeof(func*)*UBL_COUNT);
	settings->libctx = calloc(UBL_COUNT, sizeof(void*));

#if defined(WITH_ATLAS)
	// settings->call_atlas = ublas_load_atlas("path/to/atlas");
	settings->call[UBL_ATLAS] = ublas_load_driver("/panfs/panasas01/cosc/db0763/ublas/build/src/drivers/atlas/libublas-driver-atlas.so");
	settings->call[UBL_ATLAS][UBF_INIT](&settings->libctx[UBL_ATLAS]);
#endif
#if defined(WITH_CUBLAS)
	settings->call[UBL_CUBLAS] = ublas_load_driver("/panfs/panasas01/cosc/db0763/ublas/build/src/drivers/cublas/libublas-driver-cublas.so"); 
	settings->call[UBL_CUBLAS][UBF_INIT](&settings->libctx[UBL_CUBLAS]);
#endif
#if defined(WITH_MKL)
	settings->call[UBL_MKL] = ublas_load_driver("/panfs/panasas01/cosc/db0763/ublas/build/src/drivers/mkl/libublas-driver-mkl.so");
	settings->call[UBL_MKL][UBF_INIT](&settings->libctx[UBL_MKL]);
#endif
#if defined(WITH_PLASMA)
	settings->call[UBL_PLASMA] = ublas_load_driver("/panfs/panasas01/cosc/db0763/ublas/build/src/drivers/plasma/libublas-driver-plasma.so");
	settings->call[UBL_PLASMA][UBF_INIT](&settings->libctx[UBL_PLASMA]);
#endif


// #if defined(WITH_MKL)
// 	// omp_set_num_threads(settings->cores);
// #endif

	settings->ann = fann_create_from_file("../atlas-goto-mkl.net");
}

void ublas_free() {
}

int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c, double alpha, double beta) {
	if (a->cols != b->rows && a->rows != c->rows && b->cols != c->cols)
		return (int)E_MISSMATCH_DIMENSIONS;
	if (a->type != b->type && b->type != c->type)
		return (int)E_MISSMATCH_TYPES;

	int info;

	if (_ub_settings->library == UBL_AUTO) {
		// if (1)
		// 	return _ubl_plasma_gemm(a, b, c, alpha, beta);
		// else
		// 	return _ubl_cblas_gemm(a, b, c, alpha, beta);

		fann_type input[2];
		input[0] = c->cols;
		input[1] = c->rows;
		fann_type *calc_out = fann_run(_ub_settings->ann, input);

		if (calc_out[1] > calc_out[0])
			_ub_settings->call[UBL_MKL][UBF_GEMM](_ub_settings->libctx[UBL_MKL], a, b, c, alpha, beta);
		else
			_ub_settings->call[UBL_ATLAS][UBF_GEMM](_ub_settings->libctx[UBL_ATLAS], a, b, c, alpha, beta);

		// if (calc_out[0] > calc_out[1] && calc_out[0] > calc_out[2])
		// 	return _ubl_atlas_gemm(a, b, c, alpha, beta);
		// else if (calc_out[1] > calc_out[0] && calc_out[1] > calc_out[2])
		// 	return _ubl_goto_gemm(a, b, c, alpha, beta);
		// else
		// 	return _ubl_mkl_gemm(a, b, c, alpha, beta);
		return 0;
	}

#if defined(WITH_ATLAS)
	else if (_ub_settings->library == UBL_ATLAS) {
		// return _ubl_atlas_gemm(a, b, c, alpha, beta);
    	_ub_settings->call[UBL_ATLAS][UBF_GEMM](_ub_settings->libctx[UBL_ATLAS], a, b, c, alpha, beta);
	}
#endif
#if defined(WITH_CUBLAS)
	else if (_ub_settings->library == UBL_CUBLAS) {
    	_ub_settings->call[UBL_CUBLAS][UBF_GEMM](_ub_settings->libctx[UBL_CUBLAS], a, b, c, alpha, beta);
	}
#endif
#if defined(WITH_MKL)
    else if (_ub_settings->library == UBL_MKL) {
    	_ub_settings->call[UBL_MKL][UBF_GEMM](_ub_settings->libctx[UBL_MKL], a, b, c, alpha, beta);
    }
#endif
#if defined(WITH_PLASMA)
	else if (_ub_settings->library == UBL_PLASMA) {
    	_ub_settings->call[UBL_PLASMA][UBF_GEMM](_ub_settings->libctx[UBL_PLASMA], a, b, c, alpha, beta);
	}
#endif
	return 0;
}
