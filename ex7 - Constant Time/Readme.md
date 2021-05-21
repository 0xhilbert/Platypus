## Constant Time Algorithm

### Description

In this exercise, we attack a constant time algorithm with a simulated SGX-Step framework.
As configuring SGX-Step can take a long time, we instead use a modified victim and therefore "cheat" to gain instruction-level precision of the victim.

### Victim

The victim function looks like the following C function.
Note that the real assembly in `enclave.cpp` is modified to allow single and zero stepping.

    uint64_t global_key[] = { 0, -1, -1, 0, 0, -1, 2 };

    uint64_t victim() {
      uint64_t *key = global_key;
      uint64_t x = 0xdeadc0de;
      uint64_t y = 1;
      do {
        uint64_t bit = *key;               // A-1
        uint64_t mult = bit != 0 ? x : 1;  // A-2

        y = y * y;                         // A-3
        y = y * mult;                      // A-4

        key += 1;                          // A-5
      } while (*key != 2);
      return y;
    }

The victim function performs constant time move operation to either fill the `mult` variable with `y` or `1`.
Therefore, if the key bit is one a `square and multiple` and if the key bit is zero just a `mulitply` is executed.

### Attack

With the simulated SGX-Step framework, we get a callback after each instruction, allowing us to decide if we want to perform a zero-step or a single-step.

* zero-step: Replays the last instruction
* single-step: Continues with the next instruction

We record each of the instructions marked in the victim function over `2097150` zero-steps in order to achieve the resolution we need for data operands to show.

In the attack we record the power consumption of each individual instruction inside the victim function over multiple traces.
The recorded samples are then analysed with SPA.

### Example outputs
As this experiment takes longer, we provided sample files in the `samples`folder.

    python3 plot.py samples/good_1.csv
    python3 plot.py samples/good_2.csv