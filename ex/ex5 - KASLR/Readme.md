## Kernel Address Space Layout Randomization Break!

### Description
In this example, we use the RAPL to break KASLR.
We use TSX to suppress the exception when accessing the kernel address range and measure the energy consumption.

### Usage

    make run
