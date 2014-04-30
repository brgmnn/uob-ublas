#include <ublas.h>

ublas_settings *_ub_settings;

/*		UBLAS initialisation
 *---------------------------------------------------------------------------*/
void ublas_init(ublas_settings *settings) {
	_ub_settings = settings;

	settings->call = malloc(sizeof(func*)*UBL_COUNT);
	settings->libctx = calloc(UBL_COUNT, sizeof(void*));

#if defined(WITH_ATLAS)
	settings->call[UBL_ATLAS] = ublas_load_driver("./src/drivers/atlas/libublas-driver-atlas.so");
	settings->call[UBL_ATLAS][UBF_INIT](&settings->libctx[UBL_ATLAS]);
#endif
#if defined(WITH_CUBLAS)
	settings->call[UBL_CUBLAS] = ublas_load_driver("./src/drivers/cublas/libublas-driver-cublas.so"); 
	settings->call[UBL_CUBLAS][UBF_INIT](&settings->libctx[UBL_CUBLAS]);
#endif
#if defined(WITH_MKL)
	settings->call[UBL_MKL] = ublas_load_driver("./src/drivers/mkl/libublas-driver-mkl.so");
	settings->call[UBL_MKL][UBF_INIT](&settings->libctx[UBL_MKL]);
#endif
#if defined(WITH_PLASMA)
	settings->call[UBL_PLASMA] = ublas_load_driver("./src/drivers/plasma/libublas-driver-plasma.so");
	settings->call[UBL_PLASMA][UBF_INIT](&settings->libctx[UBL_PLASMA]);
#endif

	settings->ann = fann_create_from_file("../neural.net");
}

/*		UBLAS clean up
 *---------------------------------------------------------------------------*/
void ublas_free() {
	ublas_settings *settings = _ub_settings;

#if defined(WITH_ATLAS)
	settings->call[UBL_ATLAS][UBF_FREE](&settings->libctx[UBL_ATLAS]);
#endif
#if defined(WITH_CUBLAS)
	settings->call[UBL_CUBLAS][UBF_FREE](&settings->libctx[UBL_CUBLAS]);
#endif
#if defined(WITH_MKL)
	settings->call[UBL_MKL][UBF_FREE](&settings->libctx[UBL_MKL]);
#endif
#if defined(WITH_PLASMA)
	settings->call[UBL_PLASMA][UBF_FREE](&settings->libctx[UBL_PLASMA]);
#endif

	// free the driver context table
	for (int i=1; i<UBL_COUNT; i++) {
		free(settings->libctx[i]);
	}
	free(settings->libctx);

	// free the call table
	for (int i=1; i<(int)UBL_COUNT; i++) {
		for (int j=0; j<(int)UBF_COUNT; j++) {
			// free(settings->call[i][j]);
		}
		free(settings->call[i]);
	}
}

/******************************************************************************
 *	BLAS		Level 3
 *****************************************************************************/

/*		GEMM
 *---------------------------------------------------------------------------*/
int ublas_gemm(ublas_matrix *a, ublas_matrix *b, ublas_matrix *c,
		double alpha, double beta) {

	if (a->cols != b->rows && a->rows != c->rows && b->cols != c->cols)
		return (int)E_MISSMATCH_DIMENSIONS;
	if (a->type != b->type && b->type != c->type)
		return (int)E_MISSMATCH_TYPES;

	int info, reset = 0;

	if (_ub_settings->library == UBL_AUTO) {
		reset = 1;

		// preliminary matrix heuristic size stuff
		if (a->cols*c->rows*c->cols < UBLAS_HEURISTIC_THRESHOLD) {
			_ub_settings->library = UBLAS_SMALL_MATRIX_LIB; 
		} else {
			// else we use the neural network for all other cases.
			float input[2];
			input[0] = c->cols;
			input[1] = c->rows;
			float *out = fann_run(_ub_settings->ann, input);

			ublas_library libbest = UBL_AUTO;
			float out_best = FLT_MIN;

			for (int i=1; i<(int)UBL_COUNT; i++) {
				if (out[i-1] > out_best) {
					_ub_settings->library = (ublas_library)i;
					out_best = out[i-1];
				}
			}
		}
	}

#if defined(WITH_ATLAS)
	if (_ub_settings->library == UBL_ATLAS) {
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

	if (reset)
		_ub_settings->library = UBL_AUTO;
	
	return 0;
}

/*		Fastest Floats
 * returns the library with the fastest time in an array of floats representing
 * execution time.
 *---------------------------------------------------------------------------*/
ublas_library ublas_ffastest(float *times) {
	ublas_library libbest = UBL_AUTO;
	float t_best = FLT_MAX;

	for (int i=1; i<(int)UBL_COUNT; i++) {
		if (times[i-1] < t_best) {
			libbest = (ublas_library)i;
			t_best = times[i-1];
		}
	}

	return libbest;
}

/*		Fastest Floats
 * returns the library with the fastest time in an array of doubles
 * representing execution time.
 *---------------------------------------------------------------------------*/
ublas_library ublas_dfastest(double *times) {
	ublas_library libbest = UBL_AUTO;
	double t_best = DBL_MAX;

	for (int i=1; i<(int)UBL_COUNT; i++)
		if (times[i-1] < t_best) {
			libbest = (ublas_library)i;
			t_best = times[i-1];
		}

	return libbest;
}
