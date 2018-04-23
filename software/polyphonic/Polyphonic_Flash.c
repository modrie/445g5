#include "Polyphonic_Flash.h"
#include <math.h>

void print_float_array(float* p, int len_p){

	for(int i = 0 ; i < len_p ; i++){
		printf("%f, ", p[i]);
	}
	printf("\n\n");
}

void print_int_array(int16_t* p, int len_p){

	for(int i = 0 ; i < len_p ; i++){
		printf("%d, ", p[i]);
	}
	printf("\n\n");
}


// python range(bottom, top) method 
int16_t* range_int(int bottom, int top){
	int len = top - bottom;
	// if (bottom == 0){
	// 	len += 1;
	// }	

	int16_t* ptr;
	ptr = (int16_t*)malloc(sizeof(int16_t)*len);
	for(int i = 0 ; i < len; i++){
		ptr[i] = bottom + i;
	}
	return ptr;
}

// python range(bottom, top) method 
float* range_float(int bottom, int top){
	int len = top - bottom;
	// if (bottom == 0){
	// 	len += 1;
	// }	

	float* ptr;
	ptr = (float*)malloc(sizeof(float)*len);
	for(int i = 0 ; i < len; i++){
		ptr[i] = (float)bottom + i;
	}
	return ptr;
}

// Performs in-place linspace on vec 
void linspace(float* vec, float startVal, float endVal, int16_t steps, uint16_t startIdx){
	if (steps == 0){
		printf("linspace does nothing is steps = 0");
		return;
	}

	if (steps == 1){
		vec[startIdx] = startVal;
		return;
	}

	float delta = (endVal - startVal)/(steps - 1);
	for (int i = 0 ; i < steps ; i++){
		vec[i+startIdx] = startVal + delta*i;
	}
}

// Returns the max value in a vector of given length
float max(float* vec, int16_t len){

	float maxVal = vec[0]; // initialize to first value in array
	if (len > 1){
		for (int i = 1; i < len ; i++ ){
			if (vec[i] > maxVal){
				maxVal = vec[i];
			}
		}
	}
	return maxVal;
}


// find max value at indices of a vector beteen startIdx and endIdx.
// endIdx is NOT inclusive 
// max [vec[startIdx endIdx) ) 
float maxVec(float* vec, int16_t startIdx, int16_t endIdx){
	
	float maxVal = vec[startIdx];

	
	for (int idx = startIdx; idx < endIdx ; idx++){
		if (vec[idx] > maxVal){
			maxVal = vec[idx];
		}
	}
	return maxVal;

}

// Returns max value's index 
int16_t argmax(float* vec, int16_t len){
	int16_t idx = 0; // initialize to first value in array
	for (int i = 1; i < len ; i++ ){
		if (vec[i] > vec[idx]){
			idx = i;
		}
	}
	return idx;
}

// return sum of a vector with given len
float sum_vec(float* vec, int16_t len){
	float sum = 0;
	for (int i = 0 ; i < len ; i++){
		sum += vec[i];
	}
	return sum;
}


// Return frequency bin which f is closest to 
int16_t find_closest_index(float f){
	int16_t bin = (int16_t)(round((f/(SAMPLING_RATE/2))*FFT_LEN));
	return bin;
}

// applies zero pad with given padLength
// Then applies hanning window up to padLength
float* zeroPadAndHanning(int16_t* signalIn, int16_t lenSignal, int16_t padLen){

	float w;
	int totalLen = lenSignal + padLen;
	float* h = (float*) malloc(sizeof(float)*(totalLen));

	// apply hanning
	for ( int i = 0 ; i < lenSignal ; i++){
		w = 0.5 - 0.5*cos((2*M_PI*i)/(lenSignal - 1));
		h[i] = w * signalIn[i];
	}

	// zero pad 
	for (int i = lenSignal ; i < totalLen ; i ++) {
		h[i] = 0.0;
	}
	return h;
}

// Create triangular power filters given center bin frequencies and FFT bin frequencies
// @cb - center bin frequencies to create triangular filters
// 			cb[0] - rising leg of first filter
// 			cb[1] - first center freq
// 			cb[31]- last center freq
// 			cb[32]- falling leg of last filter
// @binFreqs - FFT bin frequencies 
//
// Returns: 
//   	Hb - triangular filters where Hb[i][j] is ith's filter's value at its j'th index
float** triangular_filters(int16_t cbLen, float* cb, float* binFreqs){
	// allocate memory
	float** Hb = (float**)malloc(sizeof(float*) * (cbLen - 2));
	

	for ( int i = 0 ; i < cbLen - 2 ; i ++ ){
		Hb[i] = (float*)malloc(sizeof(float) * FFT_LEN);
	}

	// zero 2d array 
	for (int i = 0 ; i < cbLen - 2 ; i ++){
		for (int j = 0 ; j < FFT_LEN ; j++){
			Hb[i][j] = 0;
		}
	}

	int m, l, k, j;
	// calculate filters 
	for (int i = 1 ; i < cbLen - 1 ; i++){
		m = find_closest_index(cb[i-1]);
		l = find_closest_index(cb[i]);
		k = find_closest_index(cb[i+1]);
		j = m;

		while(binFreqs[j] <= binFreqs[k]){
			// rising leg
			if (binFreqs[j] <= cb[i]){
				Hb[i-1][j] = (binFreqs[j] - binFreqs[m]) / (binFreqs[l]-binFreqs[m]);
			}
			// falling leg 
			else{
				Hb[i-1][j] = (binFreqs[k]-binFreqs[j])  /  (binFreqs[k]-binFreqs[l]);
			}
			j += 1;
		}
	}

	return Hb;
}


float* get_f0_cands(void){
    
    // f0index = find(constants.freq <= 1500 & constants.freq >= 60);
    // constants.f0cands = constants.freq(constants.f0index);
    // Create f0 candidate frequencies (http://www.phy.mtu.edu/~suits/NoteFreqCalcs.html)
    
    float f0 = 55.0;        				 //Hz, A three octaves below A above the middle C
    float* f0Cands  = range_float(0, NUM_F0_CANDS);  //Five octaves of candidate notes. Use half step to get out of tune freqs
    float a  = pow(2.0,(1.0/12.0));		
    
    for (int i = 0 ; i < NUM_F0_CANDS ; i++) {
    	f0Cands[i] = f0*(pow(a, f0Cands[i]));
    }
    return f0Cands;
}


// Calculates the number of harmonics to analyze for each f0Cand so that 
// harmonics < BW 


int16_t* num_harmonics_for_f0s(int16_t numHarmonics, float* f0Cands){
	float freq; 
	int16_t numHarmonicsForF0;
	int16_t* f0CandsNumHarmonics = (int16_t*)malloc(sizeof(int16_t) * NUM_F0_CANDS);

	for (int i = 0 ; i < NUM_F0_CANDS; i++){
		// Determine number of harmonics within BW for each F0 
		// and store in f0CandsNumHarmonics
		freq = (numHarmonics + 1) * f0Cands[i];               
		numHarmonicsForF0 = numHarmonics;

		// find number of harmonics to keep frequncies < BW 
		while(freq >= SAMPLING_RATE/2){
			numHarmonicsForF0--;
			freq = f0Cands[i]*(numHarmonicsForF0+1);
		}

		// store in ptrs
		if (numHarmonicsForF0 == numHarmonics) {
			f0CandsNumHarmonics[i] = numHarmonicsForF0;	
		}
		else{
			f0CandsNumHarmonics[i] = numHarmonicsForF0 + 1;	 // why this? 
		}
	}
	return f0CandsNumHarmonics;
}



// Calculates the fundamental and harmonic bins for each f0 candidate
// @halfBinWidth          		 - half bin width (Hz) 
// @numHarmonics 				 - The # of harmonics to get bin indices for 
// @f0Cands 					 - list of f0 candidate frequencies 

/****  updated in method *****/
//  @f0CandsHarmonicNumBins      - number of bins each harmonic of each f0 are
								  
//  f0CandsHarmonicNumBins[i][j] - number of bin indices for jth harmonic of ith F0 candidate
//  f0CandsNumHarmonics  	     - number of harmonics within BW for each f0 
//							    		 - this ptr is updated in the function 
// f0CandsNumHarmonics[i]       		 - number of harmonics analyzed for f0cand[i]
/*****************************/

//  @retval The fundamental and harmonic bins associated with each f0 candidate 
//  candidateFreqBins[i][j][k] 	 - kth bin index for jth harmonic for ith F0 candidate


int16_t** get_candidate_freq_bins(float halfBinWidth, int16_t numHarmonics, float* f0Cands, int16_t* f0CandsNumHarmonics, int16_t** f0CandsHarmonicNumBins){

	float freq, lowFreq, highFreq;
	int16_t lowFreqBin, highFreqBin;
	int16_t numHarmonicsForF0;
	int16_t freqBinsLen;
	int16_t lenHarmonicIndices;
	int16_t** candidateFreqBinStart = (int16_t**) malloc(sizeof(int16_t*) * NUM_F0_CANDS);
	int16_t h;
	
	// Determine bins for f0 and its associated harmonics for each f0 candidate
	for (int i = 0 ; i < NUM_F0_CANDS ; i++){
		int16_t* binIndices = NULL;
		freqBinsLen = 0;

		// allocate memory for bins
		candidateFreqBinStart[i] = (int16_t*) malloc(sizeof(int16_t) * f0CandsNumHarmonics[i]);
		f0CandsHarmonicNumBins[i] = (int16_t*)malloc(sizeof(int16_t*)*f0CandsNumHarmonics[i]);

		for (h = 1; h < f0CandsNumHarmonics[i] + 1  ; h++){

			freq = f0Cands[i]*h;

				if (i > 5 && i < NUM_F0_CANDS - 1){
					lowFreq = (f0Cands[i-1]*h + freq)/2.0;
					highFreq = (f0Cands[i+1]*h + freq)/2.0;
				}
				else{
					lowFreq = freq - halfBinWidth;
					highFreq = freq + halfBinWidth;
				}
				lowFreqBin = find_closest_index(lowFreq);
				highFreqBin = find_closest_index(highFreq);

				// Consider changing to ending looping through harmonics 
				if (highFreqBin > FFT_LEN - 1){
					highFreqBin = FFT_LEN -1;
				}
				
				// Create list of indices in between low and high frequencies
				candidateFreqBinStart[i][h-1] = lowFreqBin;

				// number of indices for this harmonic
				lenHarmonicIndices = highFreqBin + 1 - lowFreqBin;
				(f0CandsHarmonicNumBins)[i][h-1] = lenHarmonicIndices;

		}

		
	}
	printf("after\n");
	return candidateFreqBinStart;

}


// Polyphonic object initializer 
void polyphonic_init(Polyphonic* p){
	

	int16_t fallingLegLastFilter = 32;     
	//  freqBins stores frequencies at each bin 
	p -> binFreqs = range_float(0, FFT_LEN);
	for (int i = 0 ; i < FFT_LEN ; i++){
		p -> binFreqs[i] = (p -> binFreqs[i] * SAMPLING_RATE)/(2* FFT_LEN);
	}

	// cb stores center bin frequencies of triangular filter bands 
	p -> numBands = fallingLegLastFilter - 2; 
	p -> cb = range_float(1, fallingLegLastFilter + 1);
	for (int i = 0; i < fallingLegLastFilter; i ++){ 
		p -> cb[i] = 229.0*(pow(10.0, p->cb[i]/21.4) - 1);	// formula for calculating triangular bin frequencies
	}

	p -> Hb = triangular_filters(p -> numBands + 2, p -> cb, p -> binFreqs);
	
	p -> f0Cands = get_f0_cands();
	
	p -> halfBinWidth = ((SAMPLING_RATE/2.0)/FFT_LEN)/2.0; 

	// analyze up to 30 harmonics for each f0 candidate
	p -> numHarmonics = 30;
	// Get number of harmonics for each F0

	p -> f0CandsNumHarmonics = num_harmonics_for_f0s(p -> numHarmonics, p -> f0Cands);

	// Get bin indices to analyze for each f0 candidate. DOUBLE CHECK INDICES NOT MATCHING PYTHON
	// p -> f0CandsNumHarmonics = (int16_t*)malloc(sizeof(int16_t) * NUM_F0_CANDS);
	p -> f0CandsHarmonicNumBins = (int16_t**)malloc(sizeof(int16_t*) * NUM_F0_CANDS);
	
	p -> f0CandsFreqBinStart = get_candidate_freq_bins(p -> halfBinWidth, p -> numHarmonics, p -> f0Cands, p -> f0CandsNumHarmonics, p -> f0CandsHarmonicNumBins);
	
	// init weights 
	p -> alpha = 27.0;
	p -> beta = 320.0;
	p -> dee = 1.0;
}


void polyphonic_deinit(Polyphonic* p){
	free(p -> cb);
	p -> cb = NULL;
	free(p -> binFreqs);
	p -> binFreqs = NULL;

	for (int i = 0 ; i < p -> numBands; i ++){
		free(p -> Hb[i]);
		p -> Hb[i] = NULL;
	}
	free(p -> Hb);
	p -> Hb = NULL;
	free (p -> f0Cands);
	p -> f0Cands = NULL;


	for(int i = 0 ; i < NUM_F0_CANDS ; i ++){	
		free(p -> f0CandsFreqBinStart[i]);
		p -> f0CandsFreqBinStart[i] = NULL;
		free(p -> f0CandsHarmonicNumBins[i]);
		p -> f0CandsHarmonicNumBins[i] = NULL;
	}
	free(p -> f0CandsFreqBinStart);
	p -> f0CandsFreqBinStart= NULL;
	free(p -> f0CandsHarmonicNumBins);
	p -> f0CandsHarmonicNumBins = NULL;
	free(p -> f0CandsNumHarmonics);
	p -> f0CandsNumHarmonics = NULL;


}

// Performs spectral whitening by removing timbral characteristics from signal
// @dataIn   - frequency domain data
// @numBands - number of triangular filter bands
// @Hb       - triangular power filters 
// @cb       - center frequencies of triangular filters
// @whitened - computed whitened signal, computed inside function

void whiten(float* dataIn, int16_t numBands, float**Hb, float*cb, float* whitened){
	
	// calculate std dev and bandwise compression coefficients for each center band frequency
	float stdb[numBands];
	float gammab[numBands];
	float sum;

	for (int i = 0 ; i < numBands ; i++){
		sum = 0;
		for (int idx = 0; idx < FFT_LEN ; idx++){
			sum += Hb[i][idx] * pow(dataIn[idx],2.0);
		}
		stdb[i] = pow(sum/FFT_LEN, 0.5);
		gammab[i] = pow(stdb[i], 0.33-1);
	}

	
	// Interpolate gamma for whole bandwith
	float gamma[FFT_LEN];
	int16_t first_band = find_closest_index(cb[1]);
	int16_t last_band = find_closest_index(cb[numBands]);
	int16_t startF, endF; 

	// Before first filter band
	for (int idx = 0; idx < first_band + 1; idx++){
		gamma[idx] = gammab[0];
	}

	// BW
	for (int idx = 1; idx < numBands ; idx++ ){
		startF = find_closest_index(cb[idx]);
		endF = find_closest_index(cb[idx+1]);
		linspace(gamma, gammab[idx - 1], gammab[idx], endF-startF+1, startF);
	}

	// After last filter band
	for (int idx = last_band ; idx < FFT_LEN ; idx++){
		gamma[idx] = gammab[numBands- 1];
	}


	// Apply gamma interpolation onto signal
	for(int i = 0; i < FFT_LEN; i++){
		whitened[i] = gamma[i]*dataIn[i];
	}

}


// // Return frequency data of given time signal
float* time_to_freq(int16_t* signalIn, int16_t numBands, float**Hb, float*cb){ // CHECK ARG TYPE
	// window and apply trailing zeros to help with interpolation
	float* hannWindowed = zeroPadAndHanning(signalIn, FFT_LEN, FFT_LEN);
	printf("HI\n");

	// print_float_array(hannWindowed, 2*FFT_LEN);
	// float* fftSignal = mag_real_fft(hannWindowed);
	float*fftSignal = BFreq;

	// fftSignal[0] = fftSignal[0]/2; # UNCOMMENT LATER 
    free(hannWindowed);
	float* whitened = (float*) malloc(sizeof(float)*FFT_LEN);
	whiten(fftSignal, numBands, Hb, cb, whitened);

	return whitened;

}


// Computes salience for each f0 candidate 
float* get_salience(float* whitened, float* f0Cands, int16_t** f0CandsFreqBinStart, int16_t* f0CandsNumHarmonics, int16_t** f0CandsHarmonicNumBins, float alpha, float beta){
	
	float sumVal;  				 // running sum for salience calculation
	float freq;					 // current f0 candidate
	float maxAmp;				 // max amplitude in spliced whitened spectrum
	float weightedAmplitude;	 // max amplitude after weighting

	int16_t harmonicBinStartIdx, harmonicBinEndIdx, numBins;

	float copy[FFT_LEN];
	for (int i =0 ; i < FFT_LEN ; i++){
		copy[i] = whitened[i];
	}

	// each f0 candidate has a salience value
	float* salience = (float*)malloc(sizeof(float) * NUM_F0_CANDS);

	// iterate through f0s
	for(int i = 0 ; i < NUM_F0_CANDS ; i++){
		sumVal = 0.0;
		freq = f0Cands[i];
		salience[i] = 0.0;

		// printf("i %d\n", i );
		// iterate through each f0's harmonics 
		for (int h = 0 ; h < f0CandsNumHarmonics[i] ; h++ ){

			// get bin indices whilere this harmonic exists
			harmonicBinStartIdx = f0CandsFreqBinStart[i][h];
			numBins = f0CandsHarmonicNumBins[i][h];
			
			// printf("harmonic bins %d\n", numBins);
		
			// print_int_array(f0CandsFreqBins[i][h], numBins);
			harmonicBinEndIdx = harmonicBinStartIdx + numBins;
			
			// printf("start %d\n", harmonicBinStartIdx);
			// printf("end %d\n", harmonicBinEndIdx);
			// Find the maximum value in the harmonic's bins 
			maxAmp = maxVec(whitened, harmonicBinStartIdx, harmonicBinEndIdx);
			
			// printf("maxAmp %f", maxAmp);
			// apply weighting and sum
			weightedAmplitude= ((freq + alpha)/(h *freq + beta)) * maxAmp;
			sumVal += weightedAmplitude;
	
		}

		// store salience value for f0[i]
		
		salience[i] = sumVal;
		// printf("2\n");
		// printf("sum %f\n", sumVal);
		// printf("1\n");

	}

	return salience;
}


// Performs in place cancellation of a fundamental freuqency located at index
void cancel_pitch( float* whitened, float* detectedFreqs, float* binFreqs, int16_t index, int16_t** f0CandsFreqBinStart, int16_t* f0CandsNumHarmonics, int16_t** f0CandsHarmonicNumBins, float alpha, float beta){
	
	int16_t numHarmonics = f0CandsNumHarmonics[index];		
	int16_t startBin, endBin, numBins;				
	
	// Perform cancellation at each harmonic
	for (int h = 0 ; h < numHarmonics ; h++){

			startBin = f0CandsFreqBinStart[index][h];
			endBin = startBin + f0CandsHarmonicNumBins[index][h];
		
			// apply cancellation at each associated bin 
			for (int bin = startBin ; bin < endBin ; bin++){
				detectedFreqs[bin] += (binFreqs[bin] + alpha)/(h*binFreqs[bin] + beta) * whitened[bin];
				whitened[bin] -= detectedFreqs[bin];
				// do not allow negative values 
				if (whitened[bin] < 0.0){
					whitened[bin] = 0.0;
				}
			}
	}


}

// Main pitch detection method 
// Returns the number of pitches detected
// @p - polyphonic initialized object
// @signalIn - time domain signal 
// @F0s      - detected F0s will updated in this method. Should be of size MAX_NOTES
int16_t pitch_detection(Polyphonic* p, int16_t* signalIn, float* F0s){
	
	// array for detected F0s
	int16_t numDetected = 0;				// number of detected F0s
	int16_t f0Index;						// index of currently detected F0
	float sMax = 0;							
	float S[MAX_NOTES] = {0,0,0,0,0,0};		
	float* salience; 
	float sumDetectedFreqs;

	// array for spectrum of detected frequencies 
	float  detectedFreqs[FFT_LEN];
	for(int i = 0; i < FFT_LEN ; i++){
		detectedFreqs[i] = 0.0;
	}


	// time -> frequency
	float* whitened = time_to_freq(signalIn, p -> numBands, p -> Hb, p -> cb);

	// iteratively detect notes
	while (S[numDetected] >= sMax){
		numDetected += 1;
		salience = get_salience(whitened, p -> f0Cands, p -> f0CandsFreqBinStart, p -> f0CandsNumHarmonics, p -> f0CandsHarmonicNumBins, p -> alpha, p -> beta);
		
		// f0 candidate is the one largest salience 
		//	f0Index = argmax(salience, FFT_LEN);
		f0Index = argmax(salience, NUM_F0_CANDS);
		// only select if greater than threshold
		if (salience[f0Index] > MIN_SALIENCE){
			break;
		}

		// frequency cancellation
		cancel_pitch(whitened, detectedFreqs, p -> binFreqs, f0Index, p -> f0CandsFreqBinStart, p -> f0CandsNumHarmonics, p -> f0CandsHarmonicNumBins, p -> alpha, p -> beta);
		
		// compute termination variable
		sumDetectedFreqs = sum_vec(detectedFreqs, FFT_LEN);
		S[numDetected] = sumDetectedFreqs / pow(numDetected, 0.7);

		// update S 
		if (S[numDetected] > sMax){
			sMax = S[numDetected];
			F0s[numDetected - 1] = p -> f0Cands[f0Index];
		}

	}

	free(whitened);
    free(salience);
	return numDetected - 1;
}


int main(){


	float F0s [MAX_NOTES] = {0,0,0,0,0,0};
	Polyphonic p;
	int16_t numDetected = 0; 
	polyphonic_init(&p);
	

	numDetected = pitch_detection(&p, B, F0s);

	print_float_array(F0s, numDetected);
	polyphonic_deinit(&p);

	
	// int i = 20; 
	// for (int  j = 0; j < p.f0CandsNumHarmonics[i]; j++){
	// 	print_int_array(p.f0CandsFreqBins[i][j], p.f0CandsHarmonicNumBins[i][j]);
	// }
	
	// float vec[10] = {321,400,6,25,6,50,50,5,2,6};
	// float max = maxVec(vec, 0, 2);
	// printf("%f\n", max);
	// linspace(vec, 3, 10, 1, 5);
	// print_float_array(v, 2);

	return 0;
}