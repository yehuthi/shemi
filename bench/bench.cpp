#include <benchmark/benchmark.h>
#include "shemi.h"
#include "../test/test_data.h"

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

static void BM_phoenician_convert_inter_string_scalar_alphabet(
	benchmark::State &state
) {
	char32_t phnx[sizeof(PHNX)];
	for (auto _ : state) {
		memcpy(phnx, PHNX, sizeof(PHNX));
		benchmark::DoNotOptimize(phnx);
		benchmark::ClobberMemory();
		_shemi_phoenician_convert_inter_string_scalar(
			phnx, PHNX_LEN,
			SHEMI_PHNX, SHEMI_ARMI
		);
		benchmark::ClobberMemory();
	}
}

static void BM_phoenician_convert_inter_string_avx2_alphabet(
	benchmark::State &state
) {
	alignas(32) char32_t phnx[sizeof(PHNX)];
	for (auto _ : state) {
		memcpy(phnx, PHNX, sizeof(PHNX));
		benchmark::DoNotOptimize(phnx);
		benchmark::ClobberMemory();
		_shemi_phoenician_convert_inter_string_avx2(
			phnx, PHNX_LEN,
			SHEMI_PHNX, SHEMI_ARMI
		);
		benchmark::ClobberMemory();
	}
}

BENCHMARK(BM_hebrew_convert);
BENCHMARK(BM_phoenician_convert_inter);
BENCHMARK(BM_phoenician_convert_inter_string_scalar_alphabet);
BENCHMARK(BM_phoenician_convert_inter_string_avx2_alphabet);