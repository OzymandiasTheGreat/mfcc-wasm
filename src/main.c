#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <emscripten.h>

#include "../kissfft/kiss_fft.h"
#include "mfcc.h"



unsigned int FRAME_SIZE;
unsigned int SAMPLE_RATE;
unsigned int WINDOW_TYPE;
kiss_fft_cfg FFT_CFG;
kiss_fft_cpx* FFT_IN;
kiss_fft_cpx* FFT_OUT;


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


double* hamming(unsigned int n) {
	const double coef[2] = { 0.54, -0.46 };
	return cosine_window(n, coef, sizeof(coef) / sizeof(double));
}


EMSCRIPTEN_KEEPALIVE void init_mfcc(unsigned int frame_size, unsigned int sample_rate) {
	FRAME_SIZE = frame_size;
	SAMPLE_RATE = sample_rate;

	FFT_CFG = kiss_fft_alloc(FRAME_SIZE, 0, 0, 0);
	FFT_IN  = malloc(FRAME_SIZE * sizeof(kiss_fft_cpx));
	FFT_OUT = malloc(FRAME_SIZE * sizeof(kiss_fft_cpx));
}


EMSCRIPTEN_KEEPALIVE void free_mfcc() {
	free(FFT_CFG);
	free(FFT_IN);
	free(FFT_OUT);
}


EMSCRIPTEN_KEEPALIVE double* get_mfcc(double* frame, unsigned int filterBanks, unsigned int m) {
	double* window = hamming(FRAME_SIZE);
	double* magnitudes = malloc(FRAME_SIZE * sizeof(double));
	double* mfccs = malloc(m * sizeof(double));

	for (unsigned int i = 0; i < FRAME_SIZE; i++) {
		FFT_IN[i].r = (double) (frame[i] * window[i]);
		FFT_IN[i].i = 0.0;
	}
	kiss_fft(FFT_CFG, FFT_IN, FFT_OUT);

	for (unsigned int i = 0; i < FRAME_SIZE; i++) {
		magnitudes[i] = sqrt((FFT_OUT[i].r * FFT_OUT[i].r) + (FFT_OUT[i].i * FFT_OUT[i].i));
	}

	mfcc(mfccs, magnitudes, SAMPLE_RATE, filterBanks, m, FRAME_SIZE, FRAME_SIZE);

	free(window);
	free(magnitudes);

	return mfccs;
}
