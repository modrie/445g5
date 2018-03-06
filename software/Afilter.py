import numpy as np 
import  matplotlib.pyplot as plt 

def main():
	f = np.arange(10,10000,1)
	A_filter = np.zeros(len(f) + 1)


	for i in f:
		freq = i - 10 
		A_filter[freq] = 12194**2*i**4/ ((i**2+20.62**2)*((i**2+737.9**2)*(i**2+107.7**2)**0.5)*(i**2+12194**2))

	print(A_filter)
	plt.plot(f,np.log(A_filter[0:9990]))
	plt.xlabel('Frequency ')
	plt.ylabel('Magnitude')
	plt.title('A Weighting Filter')
	plt.show()


main()


