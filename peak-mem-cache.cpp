/************Random 64 bit read test**************/
//a-spec
#include "util.h"
#include <cstdio>
#include <cstring>
#include <memory>
#include <random>
#include <algorithm>
#define L1_CACHE_SIZE 32*1024  // Size of L1 cache to be tested in bytes.
#define L2_CACHE_SIZE 256*1024 // Size of L2 cache to be tested in bytes.
#define L3_CACHE_SIZE 6*1024*1024 // Size of L3 cache to be tested in bytes.    
#define REDUCE_SIZE 0.4


using namespace std;

//RANDOM

// Define a structure "options" to store program configuration options
struct
{
  size_t repeat_times = 7; // total number of repetitions, including warmup repetitions
  size_t warmup_times = 2; // number of warmup repetitions
  bool print_each_time = true; // indicates whether the result of each repetition should be printed
  size_t array_size; // size of the array in number of elements
  size_t threads = 1; // number of threads used in parallel execution
} options;

// Declare two pointers of type 'block', called 'a' and 'b'
double* a = nullptr;

// We'll use a separate array to hold indices
size_t* indices = nullptr;

void init() {
  // Allocate aligned memory for array 'a'
  a = static_cast<double*>(aligned_alloc(64, options.array_size * sizeof(double)));

  // Allocate memory for the indices array
  indices = static_cast<size_t*>(aligned_alloc(64, options.array_size * sizeof(size_t)));

  // Fill the array 'a' with sequential values and indices with sequential numbers
  for (size_t i = 0; i < options.array_size; ++i) {
    a[i] = static_cast<double>(i);
    indices[i] = static_cast<size_t>(i);
  }

  // Shuffle the indices to randomize the order
  random_device rd;
  mt19937 g(rd());
  shuffle(indices, indices + options.array_size, g);
}

// This function reads data from an array in a random manner, following the indices stored in the elements of the array
template <bool use_memcpy>
void calculate() {
  // Use assume_aligned to tell the compiler that 'a' is aligned
  double *la = assume_aligned<64>(a);
  size_t *lindices = assume_aligned<64>(indices);

  // Iterate over all elements in a random order defined by the shuffled indices
  for (size_t i = 0; i < options.array_size; ++i) {
    // Access 'a' in a random way using shuffled indices
    size_t idx = lindices[i];
    volatile double temp = la[idx];
  }
}

template <bool use_memcpy>
void measure()
{
  long n_bytes = options.array_size * sizeof(double);
  printf("Measuring time to read %ld KiB with %zu threads %s:\n", n_bytes / 1024, options.threads, use_memcpy ? "using memcpy" : "using a loop");

  vector<double> times;
  vector<double> bps;

  for (size_t i = 0; i < options.repeat_times; ++i)
  {
    double elapsed_time = measure_time(calculate<use_memcpy>);

    if (i >= options.warmup_times)
    {
      times.push_back(elapsed_time);
      bps.push_back(n_bytes / elapsed_time);
    }

    if (options.print_each_time)
    {
      printf("    Run %2ld/%2ld: %7.10fs ⇒ %7.2f GiB/s %7.2f GB/s  %s\n", i + 1, options.repeat_times, elapsed_time, n_bytes / elapsed_time / (1024 * 1024 * 1024), n_bytes / elapsed_time / (1000 * 1000 * 1000), i < options.warmup_times ? "(warmup)" : "");
    }
  }

  double average_time = vector_average(times);
  double stddev_time = vector_stddev(times);
  printf("Average time (s): %7.2f±%.10f      threads = %zu\n", average_time, stddev_time, options.threads);

  double average_bps = vector_average_harmonic(bps);
  double stddev_bps = vector_stddev_harmonic(bps);
  printf("Average GiB/s:    %7.2f±%.2f      threads = %zu\n", average_bps / (1024 * 1024 * 1024), stddev_bps / (1024 * 1024 * 1024), options.threads);
  printf("Average GB/s:     %7.2f±%.2f      threads = %zu\n\n", average_bps / (1000 * 1000 * 1000), stddev_bps / (1000 * 1000 * 1000), options.threads);
}

void executeTest(string cache_type, size_t array_size) {
  printf("Testing %s cache\n", cache_type.c_str());
  options.array_size = array_size;
  init();
  measure<false>();  
  free(a);
  free(indices);
}

int main(int argc, char **argv)
{
  for (int i = 1; i < argc; ++i)
  {
    if (!parse_bool_arg(argv[i], "print-each-time", options.print_each_time) &&
     !parse_size_arg(argv[i], "repeat-times", options.repeat_times) &&
      !parse_size_arg(argv[i], "warmup-times", options.warmup_times) &&
       !parse_size_arg(argv[i], "array-size", options.array_size) &&
        !parse_size_arg(argv[i], "threads", options.threads))
    {
      fprintf(stderr, "Incorrect argument: %s\n", argv[i]);
      return 1;
    }
  }

  printf("Benchmark: Random reads 64 bits spec-a\n");
  executeTest("L1", (L1_CACHE_SIZE / sizeof(double)) * REDUCE_SIZE);
  executeTest("L2", (L2_CACHE_SIZE / sizeof(double)) * REDUCE_SIZE);
  executeTest("L3", (L3_CACHE_SIZE / sizeof(double)) * REDUCE_SIZE);

  return 0;
}

