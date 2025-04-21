#include <benchmark/benchmark.h>
#include <random>
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

static void fuzzy_data(char32_t *dest, size_t len) {
	std::mt19937 rng;
	std::uniform_int_distribution<char32_t> dist(
		0x10900 - 10, 0x10915 + 10
	);
	for (size_t i = 0; i < len; i++) dest[i] = dist(rng);
}

static void BM_phoenician_convert_inter_string_scalar_fuzzy(
	benchmark::State &state
) {
	const size_t size = state.range(0);
	const size_t len = size / sizeof(char32_t);
	char32_t *master = new char32_t[len];
	fuzzy_data(master, len);
	char32_t *phnx = new char32_t[len];
	for (auto _ : state) {
		memcpy(phnx, master, size);
		benchmark::DoNotOptimize(phnx);
		benchmark::ClobberMemory();
		_shemi_phoenician_convert_inter_string_scalar(
			phnx, len,
			SHEMI_PHNX, SHEMI_ARMI
		);
		benchmark::ClobberMemory();
	}
	delete[] master;
	delete[] phnx;
}

static void BM_phoenician_convert_inter_string_avx2_fuzzy(
	benchmark::State &state
) {
	const size_t size = state.range(0);
	const size_t len = size / sizeof(char32_t);
	char32_t *master = new char32_t[len];
	fuzzy_data(master, len);
	char32_t *phnx = new char32_t[len];
	for (auto _ : state) {
		memcpy(phnx, master, size);
		benchmark::DoNotOptimize(phnx);
		benchmark::ClobberMemory();
		_shemi_phoenician_convert_inter_string_avx2(
			phnx, len,
			SHEMI_PHNX, SHEMI_ARMI
		);
		benchmark::ClobberMemory();
	}
	delete[] master;
	delete[] phnx;
}

static void BM_phoenician_convert_inter_string_sse4_2_fuzzy(
	benchmark::State &state
) {
	const size_t size = state.range(0);
	const size_t len = size / sizeof(char32_t);
	char32_t *master = new char32_t[len];
	fuzzy_data(master, len);
	char32_t *phnx = new char32_t[len];
	for (auto _ : state) {
		memcpy(phnx, master, size);
		benchmark::DoNotOptimize(phnx);
		benchmark::ClobberMemory();
		_shemi_phoenician_convert_inter_string_sse4_2(
			phnx, len,
			SHEMI_PHNX, SHEMI_ARMI
		);
		benchmark::ClobberMemory();
	}
	delete[] master;
	delete[] phnx;
}

BENCHMARK(BM_hebrew_convert);
BENCHMARK(BM_phoenician_convert_inter);
BENCHMARK(BM_phoenician_convert_inter_string_scalar_alphabet);
BENCHMARK(BM_phoenician_convert_inter_string_avx2_alphabet);
BENCHMARK(BM_phoenician_convert_inter_string_scalar_fuzzy)
	->Arg(32)->Range(16, 1 << 20);
BENCHMARK(BM_phoenician_convert_inter_string_avx2_fuzzy)
	->Arg(32)->Range(16, 1 << 20);
BENCHMARK(BM_phoenician_convert_inter_string_sse4_2_fuzzy)
	->Arg(32)->Range(16, 1 << 20);
