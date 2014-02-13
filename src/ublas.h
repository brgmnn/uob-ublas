#ifndef UBLAS_H
#define UBLAS_H

#ifdef WITH_ATLAS
	#include <cblas.h>
#endif

double ublas_ddot( int n, double *x, int iix, double *y, int iiy );

#endif