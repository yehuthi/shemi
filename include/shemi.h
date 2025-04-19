#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>

// HFN (header fn)
#ifdef __cplusplus
  #if __cpp_constexpr >= 200704L
    #define HFN constexpr
  #else
    #define HFN inline
  #endif
#else
  #define HFN static inline
#endif

#define SHEMI_PHOENICIAN (0x10900)
#define SHEMI_HEBREW     (0x005D0)
#define SHEMI_ARAMAIC    (0x10840)
#define SHEMI_SAMARITAN  (0x00800)

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

#ifdef __cplusplus
}
#endif