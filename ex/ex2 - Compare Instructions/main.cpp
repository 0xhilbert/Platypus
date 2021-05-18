#include "../measure/measure.h"

#include <stdio.h>
#include <string.h>

#define S 20000
#define N 50000

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("usage: %s [imul|add|nop]\n", argv[0]);
    return -1;
  }

  // init the measurement library
  init();

  // measure imul
  if (strcmp(argv[1], "imul") == 0) {
    for (int s = 0; s < S; ++s) {
      volatile uint64_t prod = 3;
      Measurement start = measure();
      for (int i = 0; i < N; ++i) {
        asm volatile("imul $0x55, %0":"=r"(prod):"r"(prod));
      }
      Measurement stop = measure();
      Sample sample = convert(start, stop);
      fprintf(stderr, "%f\n", sample.energy);
    }
  } 
  // measure add
  else if (strcmp(argv[1], "add") == 0) {
    for (int s = 0; s < S; ++s) {
      volatile uint64_t sum = 0;
      Measurement start = measure();
      for (int i = 0; i < N; ++i) {
        asm volatile("add $0x55, %0":"=r"(sum):"r"(sum));
      }
      Measurement stop = measure();
      Sample sample = convert(start, stop);
      fprintf(stderr, "%f\n", sample.energy);
    }
  }
  // measure nop
  else if (strcmp(argv[1], "nop") == 0) {
    for (int s = 0; s < S; ++s) {
      Measurement start = measure();
      for (int i = 0; i < N; ++i) {
        asm volatile("nop");
      }
      Measurement stop = measure();
      Sample sample = convert(start, stop);
      fprintf(stderr, "%f\n", sample.energy);
    }
  }
  return 0;
}