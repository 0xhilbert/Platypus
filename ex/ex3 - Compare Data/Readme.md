## Measure the energy consumption of different x86 instructions

### Description
In this example, we use the RAPL interface to compare the energy consumption of the `imul` instruction when using different operands.

Note that this example only works if SGX is disabled or the Platypus firmware patches are not installed.

### Usage

    make run
    make plot
