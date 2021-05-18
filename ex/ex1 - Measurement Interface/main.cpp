#include "../measure/measure.h"

#include <stdio.h>

int main() {
  char buffer[200];

  // init the measurement library
  init();

  // average N runs
  int N = 50000;
  
  // measure the start "stamp"
  Measurement start = measure();

  // perform some work
  for (int i = 0; i < N; ++i) {
    snprintf(buffer, 200, "formating this message %d times consumed: ", i + 1);
  }

  // measure the end "stamp"
  Measurement stop = measure();

  // convert the differential readings into a sample
  Sample sample = convert(start, stop);

  // print the message
  puts(buffer);
  // print the sample
  print(sample); 
}