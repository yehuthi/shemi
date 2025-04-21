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

BENCHMARK(BM_hebrew_convert);