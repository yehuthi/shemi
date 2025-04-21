#include <benchmark/benchmark.h>
#include "shemi.h"

static void BM_hebrew_convert(benchmark::State &state) {
	for (auto _ : state) {
		uint32_t c = 0x05DE;
		benchmark::DoNotOptimize(c);
		benchmark::ClobberMemory();
		benchmark::DoNotOptimize(shemi_hebrew_convert(c, SHEMI_PHNX));
	}
}

static void BM_phoenician_convert_inter(benchmark::State &state) {
	for (auto _ : state) {
		uint32_t c = 0x10915;
		benchmark::DoNotOptimize(c);
		benchmark::ClobberMemory();
		benchmark::DoNotOptimize(
			shemi_phoenician_convert_inter(c, SHEMI_PHNX, SHEMI_ARMI)
		);
	}
}

BENCHMARK(BM_hebrew_convert);
BENCHMARK(BM_phoenician_convert_inter);