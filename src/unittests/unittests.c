#include <stdio.h>
#include <sput.h>
#include <ublas.h>

ublas_settings settings;

static int count_vowels(const char *s) {
	const char *cp = s;
	int count = 0;

	while (*cp) {
		if (*cp == 'a' || *cp == 'e' || *cp == 'i' || *cp == 'o' || *cp == 'u')
			count++;
		cp++;
	}
	return count;
}

// static void test_vowels_present() {
// 	sput_fail_unless(count_vowels("book")  == 2, "book == 2v");
// 	sput_fail_unless(count_vowels("hand")  == 1, "hand == 1v");
// 	sput_fail_unless(count_vowels("test")  == 1, "test == 1v");
// 	sput_fail_unless(count_vowels("Peter") == 2, "Peter == 2v");
// 	sput_fail_unless(count_vowels("Apu")   == 2, "Apu == 2v");
// }

// static void test_no_vowels_present() {
// 	sput_fail_unless(count_vowels("GCC") == 0, "GCC == 0v");
// 	sput_fail_unless(count_vowels("BBC") == 0, "BBC == 0v");
// 	sput_fail_unless(count_vowels("CNN") == 0, "CNN == 0v");
// 	sput_fail_unless(count_vowels("GPS") == 0, "GPS == 0v");
// 	sput_fail_unless(count_vowels("Ltd") == 0, "Ltd == 0v");
// }

static void test_matrix_equal() {
	float da[4]  = {1,3,4,2};
	float db[4]  = {1,3,2,4};
	float da2[4] = {1,3,4,2};

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE),
		*b = ublas_new_matrix(2, 2, db, SINGLE),
		*a2 = ublas_new_matrix(2, 2, da2, SINGLE);

	sput_fail_unless(ublas_matrix_equal(a, b) == 3, "equal(a, b) == 3");
	sput_fail_unless(ublas_matrix_equal(b, b) == 0, "equal(b, b) == 0");
	sput_fail_unless(ublas_matrix_equal(a, a2) == 0, "equal(a, a2) == 0");
}

static void test_gemm_1() {
	/**************************************************************************
	 *		2x2 * 2x2 = 2x2
	 *************************************************************************/
	float da[4] = {1,4,3,2};
	float db[4] = {2,0,0,2};
	float dc[4];
	float dbenchmark[4] = {2,8,6,4};

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE),
		*b = ublas_new_matrix(2, 2, db, SINGLE),
		*c = ublas_new_matrix(2, 2, dc, SINGLE),
		*benchmark = ublas_new_matrix(2, 2, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x2x2 multiply (ATLAS)");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x2x2 multiply (CuBLAS)");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x2x2 multiply (MKL)");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x2x2 multiply (PLASMA)");
#endif
}

static void test_gemm_2() {
	/**************************************************************************
	 *		2x5 * 5x2 = 2x2
	 *************************************************************************/
	float da[10] = {1,5,2,4,3,3,4,2,5,1};
	float db[10] = {1,0,1,0,1,0,1,0,1,0};
	float dc[4];
	float dbenchmark[4] = {9,9,6,6};

	ublas_matrix *a = ublas_new_matrix(2, 5, da, SINGLE),
		*b = ublas_new_matrix(5, 2, db, SINGLE),
		*c = ublas_new_matrix(2, 2, dc, SINGLE),
		*benchmark = ublas_new_matrix(2, 2, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x2 multiply (ATLAS)");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x2 multiply (CuBLAS)");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x2 multiply (MKL)");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x2 multiply (PLASMA)");
#endif
}

static void test_gemm_3() {
	/**************************************************************************
	 *		5x5 * 5x2 = 5x2
	 *************************************************************************/
	float da[25] = {1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5};
	float db[10] = {2,0,2,0,2,0,2,0,2,0};
	float dc[10];
	float dbenchmark[10] = {18,18,18,18,18,12,12,12,12,12};

	ublas_matrix *a = ublas_new_matrix(5, 5, da, SINGLE),
		*b = ublas_new_matrix(5, 2, db, SINGLE),
		*c = ublas_new_matrix(5, 2, dc, SINGLE),
		*benchmark = ublas_new_matrix(5, 2, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "5x5x2 multiply (ATLAS)");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "5x5x2 multiply (CuBLAS)");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "5x5x2 multiply (MKL)");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "5x5x2 multiply (PLASMA)");
#endif
}

static void test_gemm_4() {
	/**************************************************************************
	 *		2x5 * 5x7 = 2x7
	 *************************************************************************/
	float da[10] = {1,1,2,2,3,3,4,4,5,5};
	float db[35] = {7,7,7,7,7,6,6,6,6,6,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,1,1,1,1,1};
	float dc[14];
	float dbenchmark[14] = {105,105,90,90,75,75,60,60,45,45,30,30,15,15};

	ublas_matrix *a = ublas_new_matrix(2, 5, da, SINGLE),
		*b = ublas_new_matrix(5, 7, db, SINGLE),
		*c = ublas_new_matrix(2, 7, dc, SINGLE),
		*benchmark = ublas_new_matrix(2, 7, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x7 multiply (ATLAS)");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x7 multiply (CuBLAS)");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x7 multiply (MKL)");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "2x5x7 multiply (PLASMA)");
#endif
}

static void test_gemm_5() {
	/**************************************************************************
	 *		13x5 * 5x7 = 13x7
	 *************************************************************************/
	float da[65] = {1,1,1,1,1,1,1,1,1,1,1,1,1,
					2,2,2,2,2,2,2,2,2,2,2,2,2,
					3,3,3,3,3,3,3,3,3,3,3,3,3,
					4,4,4,4,4,4,4,4,4,4,4,4,4,
					5,5,5,5,5,5,5,5,5,5,5,5,5};
	float db[35] = {7,7,7,7,7,6,6,6,6,6,5,5,5,5,5,4,4,4,4,4,3,3,3,3,3,2,2,2,2,2,1,1,1,1,1};
	float dc[91];
	float dbenchmark[91] = {105,105,105,105,105,105,105,105,105,105,105,105,105,
							90,90,90,90,90,90,90,90,90,90,90,90,90,
							75,75,75,75,75,75,75,75,75,75,75,75,75,
							60,60,60,60,60,60,60,60,60,60,60,60,60,
							45,45,45,45,45,45,45,45,45,45,45,45,45,
							30,30,30,30,30,30,30,30,30,30,30,30,30,
							15,15,15,15,15,15,15,15,15,15,15,15,15};

	ublas_matrix *a = ublas_new_matrix(13, 5, da, SINGLE),
		*b = ublas_new_matrix(5, 7, db, SINGLE),
		*c = ublas_new_matrix(13, 7, dc, SINGLE),
		*benchmark = ublas_new_matrix(13, 7, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "13x5x7 multiply (ATLAS)");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "13x5x7 multiply (CuBLAS)");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "13x5x7 multiply (MKL)");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "13x5x7 multiply (PLASMA)");
#endif
}

int main(int argc, char *argv[]) {
	settings.cores = 16;
	settings.library = UBL_AUTO;
	ublas_init(&settings);

	sput_start_testing();

	// sput_enter_suite("count_vowels(): Vowels Present");
	// sput_run_test(test_vowels_present);

	// sput_enter_suite("count_vowels(): No Vowels Present");
	// sput_run_test(test_no_vowels_present);

	sput_enter_suite("Matrix Operations: Equality");
	sput_run_test(test_matrix_equal);

	sput_enter_suite("BLAS Operations: GEMM");
	sput_run_test(test_gemm_1);
	sput_run_test(test_gemm_2);
	sput_run_test(test_gemm_3);
	sput_run_test(test_gemm_4);
	sput_run_test(test_gemm_5);

	sput_finish_testing();

	return sput_get_return_value();
}