import numpy as np
import matplotlib.pyplot as plt
import sys
#import scipy.signal as sig

for input in sys.argv[1:]:

  data = np.loadtxt(input, delimiter=';')

  #N=5
  #d = sig.lfilter(np.ones((N,))/N, 1, data[1:])
  d = data

  plt.plot(data)

plt.grid()
plt.show() 
