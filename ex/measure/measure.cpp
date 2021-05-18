#include "measure.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define USE_MSR 0

static uint64_t rdtsc() {
  uint64_t a, d;
  asm volatile("mfence");
  asm volatile("rdtsc" : "=a"(a), "=d"(d));
  a = (d << 32) | a;
  asm volatile("mfence");
  return a;
}

static uint64_t calib() {
  uint64_t start = rdtsc();
  sleep(1);
  uint64_t end = rdtsc();
  return end - start;
}

static int fd;
static uint64_t rdtsc_scale;

void init() {
#if USE_MSR == 0
  fd = open("/sys/class/powercap/intel-rapl/intel-rapl:0/intel-rapl:0:0/energy_uj", O_RDONLY);
#else
  fd = open("/dev/cpu/1/msr", O_RDONLY);
#endif
  assert(fd > 0);
  rdtsc_scale = calib();
}

uint64_t read_rapl() {
#if USE_MSR == 0
  char buffer[17];
  pread(fd, buffer, 16, 0);
  buffer[16] = 0;
  return strtol(buffer, NULL, 10);
#else
  uint64_t value;
  ssize_t n = pread(fd, &value, 8, 0x00000611);
  assert(n == 8);
  return value;
#endif
}

Measurement measure() {
  Measurement m;
  m.time_stamp = rdtsc();
  m.rapl_readout = read_rapl();
  return m;
}

Sample convert(Measurement const &start, Measurement const &stop) {
  Sample s;
#if USE_MSR == 0
  s.energy = (stop.rapl_readout - start.rapl_readout) / 1000000.0;
#else
  s.energy = (stop.rapl_readout - start.rapl_readout);
#endif
  s.time   = (stop.time_stamp - start.time_stamp) / (double)rdtsc_scale;
  s.power  = s.energy / s.time;
  s.count  = 0;
  return s;
}

void add(Sample &dst, Sample const &src) {
  dst.energy += src.energy;
  dst.time += src.time;
  dst.power = dst.energy / dst.time;
  dst.count++;
}

void print(const Sample& sample) {
  printf("energy %5.5f J\n", sample.energy);
  printf("power  %5.5f W\n", sample.power);
  printf("time   %5.5f s\n", sample.time);
}

uint64_t get_timestamp() {
  return rdtsc();
}

uint64_t seconds_2_timeticks(double seconds) {
  return rdtsc_scale * seconds;
}