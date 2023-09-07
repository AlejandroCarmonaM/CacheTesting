#ifndef _util_h_
#define _util_h_

#include <vector>
#include <string>
#include <numeric>
#include <utility>
#include <ctgmath>
#include <omp.h>

bool parse_size_arg(const char* arg, const char* name, size_t& var);
bool parse_double_arg(const char* arg, const char* name, double& var);
bool parse_bool_arg(const char* arg, const char* name, bool& var);
bool parse_string_arg(const char* arg, const char* name, std::string& var);

template<typename F, typename ...Args>
double measure_time(F func, Args&&... args) {
  auto start = omp_get_wtime();
  func(std::forward<Args>(args)...);
  return omp_get_wtime() - start;
}

template<typename T>
T vector_average(const std::vector<T>& v) {
  return reduce(v.begin(), v.end(), 0.0) / v.size();
}

template<typename T>
T vector_stddev(const std::vector<T>& v) {
  T avg = vector_average(v);
  return sqrt(accumulate(v.begin(), v.end(), 0.0,
                         [=](T acc, T t){
                           T dt = avg - t;
                           return acc + dt * dt; })
              / v.size());
}

template<typename T>
T vector_average_harmonic(const std::vector<T>& v) {
  return v.size() / accumulate(v.begin(), v.end(), 0.0,
                               [=](T acc, T t){ return acc + T(1) / t; });
}

template<typename T>
T vector_stddev_harmonic(const std::vector<T>& v) {
  // F.C. Lam, C.T. Hung, D.G. Perrier, Estimation of Variance for Harmonic Mean Half-Lives, Journal of Pharmaceutical Sciences, Volume 74, Issue 2, 1985, Pages 229-231, ISSN 0022-3549, https://doi.org/10.1002/jps.2600740229.
  // Pharmaceutics 2017, 9, 14; doi:10.3390/pharmaceutics9020014
  T avg = vector_average_harmonic(v);
  T iavg = T(1) / avg;
  return sqrt(accumulate(v.begin(), v.end(), 0.0,
                         [=](T acc, T t){
                           T dt = iavg - T(1) / t;
                           return acc + dt * dt; })
              / v.size()) * avg * avg;
}

// To be able to use pragmas in macros
#define MACRO_PRAGMA(x) _Pragma(#x)

// Portable #pragma unroll
#ifdef __clang__
   // equivalent to #pragma unroll (x)
#  define PRAGMA_UNROLL(x) MACRO_PRAGMA(unroll (x)) // supported by clang and icc
#else
   // equivalent to #pragma GCC unroll (x)
#  define PRAGMA_UNROLL(x) MACRO_PRAGMA(GCC unroll (x)) // supported by clang and icc
#endif

#endif
