import numpy as np
import IPython
from scipy.io.wavfile import read
import matplotlib.pyplot as plt
import os 
import math
from scipy.fftpack import fft



# Initialize parameters 
n_fft = 4096     # fft length
n_hop = 1024      # window overlap 256 - 1024 
n_templates = 30 # number of notes in library 
template_labels = np.array(["E2", "F2", "F#2", "G2", "G#2", "A3", "A#3", "B3", "C3", "C#3", "D3", "D#3", 
                   "E3", "F3", "F#3", "G3", "G#3", "A4", "A#4", "B4", "C4", "C#4", "D4", "D#4", 
                   "E4", "F4", "F#4", "G4", "G#4", "A5"])


# Loads a .wav file and returns array of sustained data in training and testing array 

def load_wav(fp, n_recordings, record_time, begin_sustain_time, end_sustain_time):

    n_data = int(math.ceil(n_recordings/2))
   
    rate, arr = read(fp)
    print(rate)
    arr_0 = arr[0:n_recordings*record_time*rate,0]
    print(n_recordings*record_time*rate)
    print(len(arr_0))
    begin_sustain = rate * begin_sustain_time
    end_sustain = rate * end_sustain_time

    print(end_sustain-begin_sustain)

    recordings = arr_0.reshape(n_recordings, record_time*rate)
  
    sustains = recordings[:, begin_sustain:end_sustain]
   
    training = np.zeros((n_data, sustains.shape[1]))
    testing = np.zeros((n_data, sustains.shape[1]))
    training[0,:] = sustains[0,:]
    testing = np.zeros((n_data, sustains.shape[1]))
    

    for i in range(1, training.shape[0]):
        training[i,:] = sustains[2*i-1,:]
        testing[i,:] = sustains[2*i, :]

    return training, testing, end_sustain, begin_sustain, rate

# Read Template data
def get_W(training, end_sustain, begin_sustain, rate):

    window_size = n_fft
    fft_iterations = int(np.floor((end_sustain - begin_sustain) / window_size))
    training = training[:, 0:fft_iterations*window_size]
    
    training = training.reshape(training.shape[0], fft_iterations, window_size)
    print(n_fft/2)
    ffts = np.log(np.abs(fft(training, n = window_size, axis = 2)[:,:,0:int(n_fft/2)]))
   
    # W dimension: number of templates x fft length
    W = np.mean(ffts, axis=1)[:]  
#     W = W[:, 0:window_size]
    W = W.T
    
    print (np.shape(W))
    index = np.argmax(W[0,0:1000])
    freqArray = np.arange(0, int(n_fft/2), 1.0) * (rate*1.0/n_fft)
    print(freqArray[index])

    return W


# beta-divergence cost function 
# @args
# v - input sound vector 
# W - template library 
# h - activation vector 

# calculate cost by interpolating between square euclidian and 
def beta_divergence (v, W, h): 
    estimate = np.dot(W,h)
    # half euclidean distance (beta = 2)
    half_euc = 0.5*np.sum((v - estimate)**2)
    # kullback-liebler distance (beta = 1)
    k_liebler = np.sum(v*np.log(v/estimate) + v - estimate)
    cost = 0.5*(k_liebler + half_euc)

    return cost
    
    
    
# update activation matrix, h is updated in place
def update_estimate (v, W, h, beta):
    estimate = np.dot(W,h)
    v_matrix = np.outer(v, np.ones(h.shape[0]))
    numerator = np.dot( (W * v_matrix).T, np.power(estimate, (beta - 2)))
    denominator = np.dot( W.T, estimate**(beta-1) )
    h_new = h*(numerator/denominator)

    
    return h_new

    

# Iteratively update h until cost < absolute threshold and 
def iterate (cost_threshold, activation_threshold, v, W, h, beta, max_iter):
    i = 0
    cost = float('inf')
    prev_cost = -float('inf')

    while np.abs(prev_cost-cost) > cost_threshold and i < max_iter:
        prev_cost = cost
        cost = beta_divergence(v,W,h)
        
        h = update_estimate(v,W,h,beta)
        i += 1

    return h
                       

# Handling input data for test (not real time)

def detect_notes(window_size, cost_threshold, activation_threshold, v, W, h, beta):
    
    max_iter = 50000
    # Loop for overlapping fft
    result = np.zeros((30))
    num_iter = 0
    print("hi")
    
    #for i in range (0, len(v), n_hop):

     num_iter += 1
     h = np.ones(n_templates)
     h /= 10

     # #Perform fft on window 
     #    if i + window_size > len(v):
     #        time_sequence = v[i:len(v)]
     #    else: 
     #        time_sequence = v[i: i + window_size]
        
    time_sequence = v 

    ffts = fft(time_sequence, 4096)
    fft_window = np.log(abs(ffts[0:int(n_fft/2)]))
    #Iterate to find notes 
    h = iterate(cost_threshold, activation_threshold, fft_window, W, h, beta, max_iter)
    result += h
        
    result /= num_iter
    notes = template_labels[result > activation_threshold]
    return notes, result
        
        
#snr = 10
#train_data, test_data, noised_1, noised_2 = load_data(snr)
'''
path = os.getcwd()
fp = path + "/data/GuitarStrings_16k.wav"
train_data, test_data, end_sustain, begin_sustain, rate = load_wav(fp)
h = np.random.normal(0,1,n_templates)
W = get_W(train_data, end_sustain, begin_sustain, rate)

for i in range(0,30):
    #notes, h_result = test( n_fft,  0.008, 0.05, noised_1[i,0:n_fft/2], W, h, 2)
    notes, h_result = NMF( n_fft,  0.008, 0.05, test_data[i,0:int(n_fft/2)], W, h, 2)
    print("i: ", i, "output: ", np.argmax(h_result))
#     print(template_labels[np.argmax(h_result)])
'''