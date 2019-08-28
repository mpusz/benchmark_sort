#include <benchmark/benchmark.h>
#include <algorithm>
#include <vector>
#include <random>
#include <numeric>
#include <execution>

namespace {

  template<typename T>
  struct wrapper {
    T value;
    wrapper() = default;
    wrapper(T v) : value(v) {}
  };

  template<typename T>
  int cmp(const void *p1, const void *p2)
  {
    const wrapper<T>* v1 = (wrapper<T>*)p1;
    const wrapper<T>* v2 = (wrapper<T>*)p2;
    return v1->value < v2->value ? -1 : (int)(v1->value > v2->value);
  }

  template<typename T>
  std::vector<wrapper<T>> init_data(size_t size)
  {
    std::vector<wrapper<T>> v(size);
    std::iota(begin(v), end(v), 0);
    std::mt19937 g{};
    std::shuffle(v.begin(), v.end(), g);
    return v;
  }

  template<typename T>
  void BM_qsort(benchmark::State& state)
  {
    auto v = init_data<T>(state.range(0));
    for (auto _ : state)
      qsort(v.data(), v.size(), sizeof(wrapper<T>), cmp<T>);
  }

  template<typename T>
  void BM_sort(benchmark::State& state)
  {
    std::vector<wrapper<T>> v = init_data<T>(state.range(0));
    for (auto _ : state)
    std::sort(begin(v), end(v), [](auto l, auto r){ return l.value < r.value; });
  }

  template<typename T>
  void BM_sort_par(benchmark::State& state)
  {
    std::vector<wrapper<T>> v = init_data<T>(state.range(0));
    for (auto _ : state)
      std::sort(std::execution::par_unseq, begin(v), end(v), [](auto l, auto r){ return l.value < r.value; });
  }
}

BENCHMARK_TEMPLATE(BM_qsort, std::int8_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort, std::int8_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort_par, std::int8_t)->Range(1024, 32 * 1024 * 1024);

BENCHMARK_TEMPLATE(BM_qsort, std::int16_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort, std::int16_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort_par, std::int16_t)->Range(1024, 32 * 1024 * 1024);

BENCHMARK_TEMPLATE(BM_qsort, std::int32_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort, std::int32_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort_par, std::int32_t)->Range(1024, 32 * 1024 * 1024);

BENCHMARK_TEMPLATE(BM_qsort, std::int64_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort, std::int64_t)->Range(1024, 32 * 1024 * 1024);
BENCHMARK_TEMPLATE(BM_sort_par, std::int64_t)->Range(1024, 32 * 1024 * 1024);
