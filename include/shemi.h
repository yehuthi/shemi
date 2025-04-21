#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include <immintrin.h>

// HFN (header fn)
#ifdef __cplusplus
  #if __cpp_constexpr >= 200704L
    #define HFN constexpr
	#define HFN_ inline
  #else
    #define HFN inline
	#define HFN_ HFN
  #endif
#else
  #define HFN static inline
  #define HFN_ HFN
#endif

#define SHEMI_PHNX (0x10900)
#define SHEMI_HEBR (0x005D0)
#define SHEMI_ARMI (0x10840)
#define SHEMI_SAMR (0x00800)

HFN bool _shemi_char_in_range(char32_t low, char32_t high, char32_t value) {
	return low <= value && value <= high;
}
#define BLOCK_FN(NAME, START, END)                  \
	HFN bool shemi_block_##NAME(char32_t c) {       \
		return _shemi_char_in_range(START, END, c); \
	}
BLOCK_FN(hebrew             , 0x00590, 0x005FF);
BLOCK_FN(hebrew_alphabet    , 0x005D0, 0x005EA);
BLOCK_FN(hebrew_cantillation, 0x00591, 0x005AF);
BLOCK_FN(phoenician         , 0x10900, 0x1091F);
BLOCK_FN(phoenician_alphabet, 0x10900, 0x10915);
BLOCK_FN(phoenician_number  , 0x10916, 0x1091B);
BLOCK_FN(aramaic            , 0x10840, 0x1085F);
BLOCK_FN(aramaic_alphabet   , 0x10840, 0x10855);
BLOCK_FN(aramaic_number     , 0x10858, 0x1085F);
BLOCK_FN(samaritan          , 0x00800, 0x0083F);
BLOCK_FN(samaritan_alphabet , 0x00800, 0x00815);
#undef BLOCK_FN

static const char32_t _SHEMI_HEBREW_NORMALIZE_TABLE[] = {
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 10, 11, 12, 12, 13, 13, 14,
	15, 16, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
};

HFN char32_t shemi_hebrew_convert_unchecked(char32_t c, char32_t target) {
	return target + _SHEMI_HEBREW_NORMALIZE_TABLE[c - 0x05D0];
}

HFN char32_t shemi_hebrew_convert(char32_t c, char32_t target) {
	return shemi_block_hebrew_alphabet(c) ?
		shemi_hebrew_convert_unchecked(c, target) : c;
}

static const char32_t _SHEMI_HEBREW_SOFIYOT[] = {
	0x05DA, 0x05DD, 0x05DF, 0x05E3, 0x05E5
};
static const char32_t _SHEMI_HEBREW_NOT_SOFIYOT[] = {
	0x05DB, 0x05DE, 0x05E0, 0x05E4, 0x05E6
};
const uint8_t _SHEMI_HEBREW_SOFIYOT_COUNT =
	sizeof(_SHEMI_HEBREW_SOFIYOT) / sizeof(_SHEMI_HEBREW_SOFIYOT[0]);

HFN bool shemi_hebrew_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_NOT_SOFIYOT[i] == c) return true;
	return false;
}

HFN char32_t shemi_hebrew_to_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_NOT_SOFIYOT[i] == c) return c - 1;
	return c;
}

HFN char32_t shemi_hebrew_to_not_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_SOFIYOT[i] == c) return c + 1;
	return c;
}

HFN char32_t shemi_phoenician_convert_inter_unchecked(
	char32_t c, char32_t from, char32_t to
) {
	return c + (to - from);
}

HFN char32_t shemi_phoenician_convert_inter(
	char32_t c, char32_t from, char32_t to
) {
	if (c < from || c > (from + 22)) return c;
	return shemi_phoenician_convert_inter_unchecked(c, from, to);
}

HFN void _shemi_phoenician_convert_inter_string_scalar(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	for (size_t i = 0; i < len; i++)
		ptr[i] = shemi_phoenician_convert_inter(ptr[i], from, to);
}

//#if defined(__AVX2__)
HFN_ void _shemi_phoenician_convert_inter_string_avx2(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	const __m256i alef_p = _mm256_set1_epi32(from - 1);
	const __m256i taw_p = _mm256_set1_epi32(from + 22 + 1);
	const __m256i unicode_offset = _mm256_set1_epi32(to - from);

	size_t i = 0;
	for (; i + 7 < len; i += 8) {
		const __m256i chr = _mm256_loadu_si256((const __m256i*)&ptr[i]);
		const __m256i mask = _mm256_and_epi32(
			_mm256_cmpgt_epi32(chr, alef_p),
			_mm256_cmpgt_epi32(taw_p, chr)
		);
		const __m256i converted = _mm256_add_epi32(unicode_offset, chr);
		const __m256i masked = _mm256_or_si256(
			_mm256_and_si256(converted, mask),
			_mm256_andnot_si256(mask, chr)
		);
		_mm256_storeu_si256((__m256i*)&ptr[i], masked);
	}

	_shemi_phoenician_convert_inter_string_scalar(&ptr[i], len - i, from, to);
}
//#endif

HFN void shemi_phoenician_convert_inter_string(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	_shemi_phoenician_convert_inter_string_scalar(ptr, len, from, to);
}

#ifdef __cplusplus
}
#endif