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
	float da[4]  = {1.0f, 3.0f, 4.0f, 2.0f};
	float db[4]  = {1.0f, 3.0f, 2.0f, 4.0f};
	float da2[4] = {1.0f, 3.0f, 4.0f, 2.0f};

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE),
		*b = ublas_new_matrix(2, 2, db, SINGLE),
		*a2 = ublas_new_matrix(2, 2, da2, SINGLE);

	sput_fail_unless(ublas_matrix_equal(a, b) == 3, "equal(a, b) == 3");
	sput_fail_unless(ublas_matrix_equal(b, b) == 0, "equal(b, b) == 0");
	sput_fail_unless(ublas_matrix_equal(a, a2) == 0, "equal(a, a2) == 0");
}

static void test_gemm() {
	float da[4] = {1.0f, 3.0f, 4.0f, 2.0f};
	float db[4] = {2.0f, 0.0f, 0.0f, 2.0f};
	float dc[4];
	float dbenchmark[4] = {2.0f, 6.0f, 8.0f, 4.0f};

	ublas_matrix *a = ublas_new_matrix(2, 2, da, SINGLE),
		*b = ublas_new_matrix(2, 2, db, SINGLE),
		*c = ublas_new_matrix(2, 2, dc, SINGLE),
		*benchmark = ublas_new_matrix(2, 2, dbenchmark, SINGLE);

#if defined(WITH_ATLAS)
	settings.library = UBL_ATLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "ATLAS multiply.");
#endif
#if defined(WITH_CUBLAS)
	settings.library = UBL_CUBLAS;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "CuBLAS multiply.");
#endif
#if defined(WITH_MKL)
	settings.library = UBL_MKL;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "MKL multiply.");
#endif
#if defined(WITH_PLASMA)
	settings.library = UBL_PLASMA;
	ublas_gemm(a, b, c, 1.0, 0.0);
	sput_fail_unless(ublas_matrix_equal(c, benchmark) == 0, "PLASMA multiply.");
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
	sput_run_test(test_gemm);

	sput_finish_testing();

	return sput_get_return_value();
}