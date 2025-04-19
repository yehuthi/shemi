#include <benchmark/benchmark.h>
#include "shemi.h"

static void BM_hebrew_to_phoenician(benchmark::State &state) {
	for (auto _ : state) {
		uint32_t c = 0x05DE;
		benchmark::DoNotOptimize(c);
		benchmark::ClobberMemory();
		benchmark::DoNotOptimize(shemi_hebrew_to_phoenician(c));
	}
}

BENCHMARK(BM_hebrew_to_phoenician);