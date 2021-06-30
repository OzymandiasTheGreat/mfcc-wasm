#include <stdlib.h>
#include <emscripten.h>

#include "../kissfft/kiss_fft.h"
#include "../libmfcc/libmfcc.h"

#include "window.h"


enum WINDOW_FUNCTION {
	RECTANGULAR       = 0,
	HANNING           = 1,
	HAMMING           = 2,
	BLACKMAN          = 3,
	TUKEY             = 4,
};


unsigned int FRAME_SIZE;
unsigned int SAMPLE_RATE;
unsigned int WINDOW_TYPE;
kiss_fft_cfg FFT_CFG;
kiss_fft_cpx* FFT_IN;
kiss_fft_cpx* FFT_OUT;


double* get_window() {
	switch (WINDOW_TYPE) {
		case RECTANGULAR:
			return rectangular(FRAME_SIZE);
		case HANNING:
			return hanning(FRAME_SIZE);
		case HAMMING:
			return hamming(FRAME_SIZE);
		case BLACKMAN:
			return blackman(FRAME_SIZE);
		case TUKEY:
			return tukey(FRAME_SIZE, 0.5);
		default:
			return NULL;
	}
}


EMSCRIPTEN_KEEPALIVE void init_mfcc(unsigned int frame_size, unsigned int sample_rate, unsigned int window_type) {
	FRAME_SIZE = frame_size;
	SAMPLE_RATE = sample_rate;
	WINDOW_TYPE = window_type;

	FFT_CFG = kiss_fft_alloc(FRAME_SIZE, 0, 0, 0);
	FFT_IN  = malloc(FRAME_SIZE * sizeof(kiss_fft_cpx));
	FFT_OUT = malloc(FRAME_SIZE * sizeof(kiss_fft_cpx));
}


EMSCRIPTEN_KEEPALIVE void free_mfcc() {
	free(FFT_CFG);
	free(FFT_IN);
	free(FFT_OUT);
}


EMSCRIPTEN_KEEPALIVE double* get_mfcc(double* frame, unsigned int filter_banks, unsigned int m) {
	double* window = get_window();
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

	for (unsigned int i = 0; i < m; i++) {
		mfccs[i] = GetCoefficient(magnitudes, SAMPLE_RATE, filter_banks, FRAME_SIZE, i);
	}

	free(window);
	free(magnitudes);

	return mfccs;
}
