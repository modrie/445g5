
#ifndef Polyphonic_FLASH_h
#define Polyphonic_FLASH_h


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <math.h>
#import "testB3.h"

#define SAMPLING_RATE 16000
#define FFT_LEN       512
#define NUM_F0_CANDS  60
#define MAX_NOTES	  6
#define MIN_SALIENCE  400

// Polyphonic object 
typedef struct _Polyphonic {
	
	int16_t  numBands;                  // # of Triangular filter center frequencies 
	float*   cb;    					// Center bin frequencies of sub_bands 
	float**  Hb;           	    		// Triangular bandpass filters 

	float*    binFreqs;      			// Frequency values of each fft bin 
	float*  f0Cands;  	    			// Frequency bins for each f0 candidate
	float   halfBinWidth;				// Half bin width 
	int16_t   numHarmonics;			    // # of harmonics to analyze for each f0 
	int16_t** f0CandsFreqBinStart;      // each f0's harmonic start Bins
	int16_t** f0CandsHarmonicNumBins;	// stores length of f0CandsFreqBins[i][j]
	int16_t* f0CandsNumHarmonics;

	float   alpha;						// weights
	float   beta;
	float   dee; 				
} Polyphonic;

// Polyphonic object initializer 
void polyphonic_init(Polyphonic* p);
// Polyphonic object deinitializer
void polyphonic_deinit(Polyphonic* p);

// Return frequency bin which freq is closest to 
int16_t find_closest_index(float freq);


// Create and return triangular filters 
// rows - no. filters
// columns - filter length 
float** triangular_filters(int16_t cbLen, float* cb, float* binFreqs);

// Create candidate notes 
float* get_f0_cands(void);

// Performs spectral whitening by removing timbral characteristics from signal
// Data_in is frequency domain data
void whiten(float* dataIn, int16_t numBands, float**Hb, float*cb, float* whitened);

// Computes salience for each f0 candidate 
float* get_salience(float* whitened, float* f0Cands, int16_t** f0CandsFreqBins, int16_t* f0CandsNumHarmonics, int16_t** f0CandsHarmonicNumBins, float alpha, float beta);

// Performs in place cancellation of a fundamental freuqency located at index
void cancel_pitch( float* whitened, float* detectedFreqs, float* binFreqs, int16_t index, int16_t** f0CandsFreqBins, int16_t* f0CandsNumHarmonics, int16_t** f0CandsHarmonicNumBins, float alpha, float beta);
// Return frequency data of given time signal
float* time_to_freq(int16_t* signalIn, int16_t numBands, float**Hb, float*cb); // may be uint16_t*

// Main pitch detection method 

// Returns the number of pitches detected
// @p - polyphonic initialized object
// @signalIn - time domain signal 
// @F0s      - detected F0s will updated in this method. Should be of size MAX_NOTES
int16_t pitch_detection(Polyphonic* p, int16_t* signalIn, float* F0s);
	
#endif




