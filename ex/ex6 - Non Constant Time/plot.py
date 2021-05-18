import numpy as np
import matplotlib.pyplot as plt
import sys

data = np.loadtxt("log.csv", delimiter=';')

index  = data[:,0]
energy = data[:,1]
counts = data[:,2]

# average over the runs
energy = energy / counts

plt.stem(index, energy)
plt.xlabel("Instruction Index")
plt.ylabel("Energy in uJ")

plt.title("Non Constant Time Algorithm")

plt.grid()
plt.show() 