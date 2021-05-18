#include "../measure/measure.h"

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <cstring>
#include <time.h>
#include <signal.h>

#define MAX_INSTRUCTIONS 100
#define ZERO_STEPS 0xFFF
#define AVERAGE 100000

#define SINGLE_STEP 0
#define ZERO_STEP 1

// the victim function inside an simulated enclave
extern uint64_t victim();

// measurement storage
uint64_t writer = 0;
Measurement buffer[MAX_INSTRUCTIONS];
uint64_t step_counter = 0;

// this callback simulates the aep callback of SGX enclaves when using SGX-STEP
extern "C" int callback() {

  switch (step_counter) {
    // started -> record start measurement
    case 0:
      buffer[writer++] = measure();
      step_counter++;
      return ZERO_STEP;

    // not started nor finished -> continue measuring
    default:
      step_counter++;
      return ZERO_STEP;

    // finished -> record end measurement
    case ZERO_STEPS:
      buffer[writer++] = measure();
      step_counter = 0;
      return SINGLE_STEP;
  }
}

int main() {
  init();
  
  Sample samples[MAX_INSTRUCTIONS];
  memset(samples, 0, sizeof(Sample)*MAX_INSTRUCTIONS);
  
  for (int avg = 0; avg < AVERAGE; ++avg) {
    writer = 0;
    step_counter = 0;
    
    // execute the victim
    victim();
    
    // average the recorded samples
    for (uint64_t i = 0; i < writer; i+=2) {
      add(samples[i/2], convert(buffer[i], buffer[i+1]));
    }

    // stats
    if (avg % (AVERAGE/100) == 0) {
      printf("\r%3d%%", avg*100/AVERAGE + 1);
      fflush(stdout);
    }
  }
  printf("\n");

  // store the recorded samples
  for (uint64_t i = 0; samples[i].count; ++i) {
    fprintf(stderr, "%lu;%f;%lu\n", i, samples[i].energy, samples[i].count);
  }
}