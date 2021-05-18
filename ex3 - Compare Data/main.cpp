#include "../measure/measure.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define S 200000
#define N 250000

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("usage: %s factor\n", argv[0]);
    return -1;
  }

  uint64_t factor = strtol(argv[1], NULL, 10);

  // init the measurement library
  init();

  // measure imul
  for (int s = 0; s < S; ++s) {
    volatile uint64_t prod = 3;
    Measurement start = measure();
    for (int i = 0; i < N; ++i) {
      asm volatile("imul %[factor], %[result]":[result]"=r"(prod):[factor]"r"(factor));
    }
    Measurement stop = measure();
    Sample sample = convert(start, stop);
    fprintf(stderr, "%f\n", sample.energy);
  }

  return 0;
}