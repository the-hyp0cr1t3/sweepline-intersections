#include <benchmark/benchmark.h>
#include <generators.hpp>
#include <sweepline/sweepline.hpp>

using Sweepline::intersection_t;

/*
static void CustomArguments(benchmark::internal::Benchmark* b) {
    const int l1 = 3 << 6, r1 = 3 << 10, mult = 2;
    const int l2 = 1, r2 = 1000, inc = 100;

    for(int i = l1; i <= r1; i *= mult)
        for(int j = l2; j <= r2; j += inc)
            b->Args({i, j});
}
*/

static void BM_ObliqueGrid(benchmark::State& state) {
    int horiz = state.range(0);
    int verti = state.range(1);
    int n = horiz + verti;
    int m = horiz * verti;

    for(auto _ : state) {
        state.PauseTiming();

        // order seg.p and seg.q!!
        std::vector<segment_t> segments = gen_oblique_grid(horiz, verti);

        state.ResumeTiming();

        std::vector<intersection_t> result = Sweepline::find_intersections(segments);
        benchmark::DoNotOptimize(result.data());
    }

    state.counters["num_segments"] = n;
    state.counters["num_intersections"] = m;
    state.SetComplexityN(n + m);
}

// Args[0] = horizontal cnt
// Args[1] = vertical cnt
BENCHMARK(BM_ObliqueGrid)
    // ->Apply(CustomArguments)
    ->ArgsProduct({
        { 1 << 5, 1 << 7, 1 << 9 },
        { 1 << 4, 1 << 6, 1 << 8 }
    })
    ->Complexity(benchmark::oNLogN);


static void BM_OriginStar(benchmark::State& state) {
    int n = state.range(0);
    int m = 1;

    for(auto _ : state) {
        state.PauseTiming();

        // order seg.p and seg.q!!
        std::vector<segment_t> segments = gen_origin_star(n);

        state.ResumeTiming();

        std::vector<intersection_t> result = Sweepline::find_intersections(segments);
        benchmark::DoNotOptimize(result.data());
    }

    state.counters["num_segments"] = n;
    state.counters["num_intersections"] = m;
    state.SetComplexityN(n + m);
}

BENCHMARK(BM_OriginStar)
    // ->DenseRange(3, 10000, 400)
    ->DenseRange(3, 10000, 2000)
    ->Complexity(benchmark::oNLogN);

BENCHMARK_MAIN();

/*
./bench --benchmark_counters_tabular=true
./bench --benchmark_counters_tabular=true --benchmark_format=csv
*/