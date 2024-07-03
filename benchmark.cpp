#include <stddef.h>
#include <stdint.h>

#ifndef PLATFORM_WEB
#include <chrono>
#include <iostream>
#include <cstdlib>
#endif

extern "C"
{
  uint64_t state0 = 1;
  uint64_t state1 = 2;
  inline uint64_t xorshift128plus()
  {
    uint64_t s1 = state0;
    uint64_t s0 = state1;
    state0 = s0;
    s1 ^= s1 << 23;
    s1 ^= s1 >> 17;
    s1 ^= s0;
    s1 ^= s0 >> 26;
    state1 = s1;
    return state0 + state1;
  }
#ifdef PLATFORM_WEB
  inline double c_random()
  {
    return xorshift128plus() / (double)UINT64_MAX;
  }
  double js_random();
#endif

#ifndef PLATFORM_WEB
  void native_benchmark(size_t loop_n, size_t test_count)
  {
    for (int i = 0; i < test_count; i++)
    {
      double sum = 0;
      for (size_t j = 0; j < loop_n; j++)
      {
        sum += rand() / (double)RAND_MAX;
        // sum += c_random();
      }
    }
  }
#else
  void c_benchmark(size_t loop_n, size_t test_count)
  {
    for (int i = 0; i < test_count; i++)
    {
      size_t sum = 0;
      for (size_t j = 0; j < loop_n; j++)
      {
        sum += c_random();
      }
    }
  }
  void js_benchmark(size_t loop_n, size_t test_count)
  {
    for (int i = 0; i < test_count; i++)
    {
      size_t sum = 0;
      for (size_t j = 0; j < loop_n; j++)
      {
        sum += js_random();
      }
    }
  }
#endif
}

#ifndef PLATFORM_WEB
int main()
{
  const size_t test_count = 100;

  std::vector<size_t> xs = {10, 100, 1000, 10000, 100000, 1000000, 10000000, 100000000, 1000000000};
  std::vector<double> native_y;

  for (size_t i = 0; i < xs.size(); i++)
  {
    size_t element = xs.at(i);
    auto start = std::chrono::system_clock::now();
    native_benchmark(element, test_count);
    auto end = std::chrono::system_clock::now();

    auto elapsed = std::chrono::duration_cast<std::chrono::duration<double, std::milli>>(end - start) / test_count;
    std::cout << "Time: " << elapsed.count() << "ms\n"
              << std::flush;

    native_y.push_back(elapsed.count());
  }

  std::cout << "Try Print!\n"
            << std::flush;
  std::cout << "Native: [";
  for (size_t i = 0; i < xs.size(); i++)
  {
    std::cout << "(" << std::to_string(xs[i]) << "," << std::to_string(native_y[i]) << ")";
    if (i < xs.size() - 1)
    {
      std::cout << ",";
    }
  }
  std::cout << "]" << std::flush;

  return 0;
}
#endif