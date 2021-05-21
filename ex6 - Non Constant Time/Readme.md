## Non Constant Time Algorithm

### Description

In this exercise, we attack a nonconstant time algorithm with a simulated SGX-Step framework.
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
        uint64_t bit = *key;  // A-1 B-1
        if (bit == -1) {
          y = y * y;          // A-2
          y = y * x;          // A-3
        } else {
          y = y * y;          // B-2
        }
        key += 1;             // A-4 B-3
      } while (*key != 2);
      return y;
    }

The victim function performs a conditional jump based on the key bit.
Therefore, the function is not considered constant time in terms of the Intel guidelines.

### Attack

With the simulated SGX-Step framework, we get a callback after each instruction, allowing us to decide if we want to perform a zero-step or a single-step.

* zero-step: Replays the last instruction
* single-step: Continues with the next instruction

We record each of the instructions marked in the victim function over `4096` zero-steps in order to overcome the sample rate limitation of RAPL.

In the attack we record the power consumption of each individual instruction inside the victim function over multiple traces.
The recorded samples are then analysed with SPA.

As the victim performs different number of operation depending on the key bit, the resulting samples also vary based on the currently executed key.


