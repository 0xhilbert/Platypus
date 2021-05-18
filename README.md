# Platypus Attack - Educational Samples
### What is the Platypus Attack
The Platypus attack is a collection of software-based power side-channel attacks, published in November 2020.
This repository provides samples for the attacks discussed in the actual paper.

For more information please visit the official [site](https://platypusattack.com). Or you can find the paper at [here](https://platypusattack.com/platypus.pdf).

### Note
All these examples were tested on Intel CPUs, AMD CPUs also expose the RAPL interface, and therefore, these examples should work on AMD as well. Except the KASLR break which is using TSX. 

### Setup
These samples use the exposed RAPL interface, a Linux-based OS with the [PowerCap](https://www.kernel.org/doc/html/latest/power/powercap/powercap.html) framework installed.
The presence of the interface can be tested by executing:

    sudo cat /sys/class/powercap/intel-rapl:0/energy_uj 

If the above results in error, please execute the following command:

    sudo apt update
    sudo apt install powercap-utils 

All of the provided samples contain Makefiles to build the example. Therefore a basic compiler toolchain (gcc, g++, ld, make) must be installed.

If no toolchain is installed, execute:

    sudo apt update
    sudo apt install build-essential

Some of the examples provide a visual representation of the data by executing a python script. Therefore, make sure `python3` is installed:

    sudo apt update
    sudo apt install python3
    sudo apt install pip3

The plot scripts use `numpy` and `matplotlib`, install these with:

    python3 -m pip install numpy
    python3 -m pip install matplotlib
     
### Example Summary

#### Ex1
Get in touch with the measurement interface.

#### Ex2 
Compare the energy consumption of different instructions.

#### Ex3
Compare the energy consumption of different data operands.

#### Ex4
A basic covert channel which measures the energy periodically in the background.

#### Ex5 
Kernel Address Space Layout randomization break using TSX.
(This example only works when the CPU supports TSX)

#### Ex6
Using a simulation of [SGX-Step](https://github.com/jovanbulck/sgx-step) to extract secret information of a square and multiply algorithm, which is non constant time.

#### Ex7
Using a simulation of [SGX-Step](https://github.com/jovanbulck/sgx-step) to extract secret information of a square and multiply algorithm, which is constant time.
