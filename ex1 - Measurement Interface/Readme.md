## Measure the energy consumption of a C function

### Description
In this example, we get hands-on with the differential RAPL interface and use it to measure the energy consumption of the `snprintf` function.
This exercise is meant to get in touch with the used measurement library.

### Usage

    make run

### Example output

    formating this message 50000 times consumed: 
    energy 0.02930 J
    power  4.64019 W
    time   0.00631 s