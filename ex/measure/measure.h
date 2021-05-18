#pragma once

#include <stdint.h>

#define DO_FOR_N_SECONDS(_n,_x) {                               \
    uint64_t until = get_timestamp() + seconds_2_timeticks(_n); \
    while (get_timestamp() < until) {                           \
      _x;                                                       \
    }                                                           \
}

struct Measurement {
  uint64_t rapl_readout;
  uint64_t time_stamp;
};

struct Sample {
  double energy; // joule
  double power;  // watt
  double time;   // seconds
  uint64_t count;
};

void init();
Measurement measure();
Sample convert(Measurement const &start, Measurement const &stop);
void add(Sample &dst, Sample const &src);

void print(const Sample& sample);
uint64_t get_timestamp();
uint64_t seconds_2_timeticks(double seconds);
