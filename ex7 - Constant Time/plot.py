import numpy as np
import matplotlib.pyplot as plt
import sys

data = np.loadtxt(sys.argv[1], delimiter=';')

index  = data[:,0]
energy = data[:,1]
counts = data[:,2]

N = len(counts)

key = [0, 1, 1, 0, 0, 1]

# average over the runs
energy = energy / counts

colors = ['r', 'b']

plt.subplot(2,1,1)
for i in range(0,N,5):
  plt.plot(index[i:i+6], energy[i:i+6],color=colors[key[i//5]])

plt.ylabel("Energy in uJ")
plt.xlabel("Instruction Index")

plt.title("Non Constant Time Algorithm")
plt.grid()
plt.subplot(2,1,2)
for i in range(0,N,5):
  plt.plot(energy[i:i+6],color=colors[key[i//5]])

plt.xlabel("Iterations overlayed")
plt.ylabel("Energy in uJ")


plt.grid()
plt.show() 