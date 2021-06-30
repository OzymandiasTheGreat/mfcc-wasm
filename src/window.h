/*
See the note in window.c
*/

#include <stdbool.h>


double* cosine_window(unsigned int n, const double* coef, unsigned int ncoef);

double* rectangular(unsigned int n);
double* hanning(unsigned int n);
double* hamming(unsigned int n);
double* blackman(unsigned int n);
double* tukey(unsigned int n, double alpha);
