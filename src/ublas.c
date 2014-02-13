#include "ublas.h"

// calculate the dot product of a matrix
double ublas_ddot( int n, double *x, int iix, double *y, int iiy ) {
	// #ifdef WITH_ATLAS
	// 	return cblas_ddot( n, x, iix, y, iiy );
	// #endif
	return (double)(n+*x+iix+*y+iiy);
}
