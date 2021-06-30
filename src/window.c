/*
Disclaimer: I (Tomas Ravinskas) am pretty bad at math and I never really learned
C programming language. Therefore I cannot vouch for the correctness of functions
defined in this file. I have assembled these functions with the help of Wikipedia
and various bits and pieces from the internet.
*/

#define _USE_MATH_DEFINES


#include <stdlib.h>
#include <complex.h>
#include <math.h>

#include "window.h"


double* cosine_window(unsigned int n, const double* coef, unsigned int ncoef) {
	double* w = malloc(n * sizeof(double));

	if (n == 1) {
		w[0] = 1.0;
	} else {
		const unsigned int wlength = n - 1;
		for (unsigned int i = 0; i < n; i++) {
			double wi = 0.0;

			for (unsigned int j = 0; j < ncoef; j++) {
				wi += coef[j] * cos(i * j * 2.0 * M_PI / wlength);
			}

			w[i] = wi;
		}
	}
	return w;
}


double* rectangular(unsigned int n) {
	double* w = malloc(n * sizeof(double));

	for (unsigned int i = 0; i < n; i++) {
		w[i] = 1.0;
	}
	return w;
}


double* hanning(unsigned int n) {
	const double coef[2] = { 0.5, -0.5 };
	return cosine_window(n, coef, sizeof(coef) / sizeof(double));
}


double* hamming(unsigned int n) {
	const double coef[2] = { 0.54, -0.46 };
	return cosine_window(n, coef, sizeof(coef) / sizeof(double));
}


double* blackman(unsigned int n) {
	const double coef[3] = { 0.42, -0.5, 0.08 };
	return cosine_window(n, coef, sizeof(coef) / sizeof(double));
}


double* tukey(unsigned int n, double alpha) {
	double* w = malloc(n * sizeof(double));

	if (n == 1) {
		w[0] = 1.0;
	} else {
		alpha = fmax(0.0, fmin(1.0, alpha));

		for (unsigned int i = 0; i < n; i++) {
			w[i] = (cos(fmax(fabs((double) i - (n - 1) / 2.0) * (2.0 / (n - 1) / alpha) - (1.0 / alpha - 1.0), 0.0) * M_PI) + 1.0) / 2.0;
		}
	}
	return w;
}
