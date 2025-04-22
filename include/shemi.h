#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <memory.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include <immintrin.h>

// HFN (header fn)
#ifdef __cplusplus
	#if __cpp_constexpr >= 200704L
		#define HFN constexpr
		#define HFN_ inline
		#define HFD constexpr
	#else
		#define HFN inline
		#define HFN_ HFN
		#define HFD static const
	#endif
#else
	#define HFN static inline
	#define HFN_ HFN
	#define HFD static const
#endif

/** @defgroup scripts Scripts */
/** @defgroup scripts_phnx Phoenician Scripts
 * @ingroup scripts
 */

/// @brief Phoenician script
/// @ingroup scripts scripts_phnx
#define SHEMI_PHNX (0x10900)
/// @brief Hebrew script
/// @ingroup scripts
#define SHEMI_HEBR (0x005D0)
/// @brief Imperial-Aramaic script
/// @ingroup scripts scripts_phnx
#define SHEMI_ARMI (0x10840)
/// @brief Samaritan script
/// @ingroup scripts scripts_phnx
#define SHEMI_SAMR (0x00800)

HFN bool _shemi_char_in_range(char32_t low, char32_t high, char32_t value) {
	return low <= value && value <= high;
}
#define BLOCK_FN(NAME, START, END)                  \
	HFN bool shemi_block_##NAME(char32_t c) {       \
		return _shemi_char_in_range(START, END, c); \
	}
BLOCK_FN(hebrew             , 0x00590, 0x005FF)
BLOCK_FN(hebrew_alphabet    , 0x005D0, 0x005EA)
BLOCK_FN(hebrew_cantillation, 0x00591, 0x005AF)
BLOCK_FN(phoenician         , 0x10900, 0x1091F)
BLOCK_FN(phoenician_alphabet, 0x10900, 0x10915)
BLOCK_FN(phoenician_number  , 0x10916, 0x1091B)
BLOCK_FN(aramaic            , 0x10840, 0x1085F)
BLOCK_FN(aramaic_alphabet   , 0x10840, 0x10855)
BLOCK_FN(aramaic_number     , 0x10858, 0x1085F)
BLOCK_FN(samaritan          , 0x00800, 0x0083F)
BLOCK_FN(samaritan_alphabet , 0x00800, 0x00815)
#undef BLOCK_FN

HFD char32_t _SHEMI_HEBREW_NORMALIZE_TABLE[] = {
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 10, 11, 12, 12, 13, 13, 14,
	15, 16, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
};

/// @brief Converts a Hebrew character to a Phoenician-family script character
/// @param c A Hebrew alphabet character
/// @param target The target Phoenician-family script (see @ref scripts_phnx)
/// @warning The given character `c` MUST be within the Hebrew alphabet range
/// `[0x05D0-0x05EA]`
/// @see shemi_hebrew_to_phoenician
/// @return The equivalent Phoenician-equivalent for `c`
HFN char32_t shemi_hebrew_to_phoenician_unchecked(char32_t c, char32_t target) {
	return target + _SHEMI_HEBREW_NORMALIZE_TABLE[c - 0x05D0];
}

/// @brief Converts a Hebrew character to a Phoenician-family script character
/// @param c A character
/// @param target The target Phoenician-family script (see @ref scripts_phnx)
/// @see shemi_hebrew_to_phoenician_unchecked
/// @return The equivalent Phoenician-equivalent for `c`, or `c` if the given
/// character is not Hebrew.
HFN char32_t shemi_hebrew_to_phoenician(char32_t c, char32_t target) {
	return shemi_block_hebrew_alphabet(c) ?
		shemi_hebrew_to_phoenician_unchecked(c, target) : c;
}

static const char32_t _SHEMI_HEBREW_SOFIYOT[] = {
	0x05DA, 0x05DD, 0x05DF, 0x05E3, 0x05E5
};
static const char32_t _SHEMI_HEBREW_NOT_SOFIYOT[] = {
	0x05DB, 0x05DE, 0x05E0, 0x05E4, 0x05E6
};
const uint8_t _SHEMI_HEBREW_SOFIYOT_COUNT =
	sizeof(_SHEMI_HEBREW_SOFIYOT) / sizeof(_SHEMI_HEBREW_SOFIYOT[0]);

/// @brief Checks if the given character is a Hebrew sofit (final) letter.
/// @param c A character
/// @return Whether `c` is sofit.
HFN bool shemi_hebrew_is_sofit(char32_t c) {
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

HFN char32_t shemi_phoenician_to_phoenician_unchecked(
	char32_t c, char32_t from, char32_t to
) {
	return c + (to - from);
}

HFN char32_t shemi_phoenician_to_phoenician(
	char32_t c, char32_t from, char32_t to
) {
	if (c < from || c > (from + 22)) return c;
	return shemi_phoenician_to_phoenician_unchecked(c, from, to);
}

void _shemi_phoenician_to_phoenician_string_scalar(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
);

#if defined(__AVX2__)
void _shemi_phoenician_to_phoenician_string_avx2(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
);
#endif


#if defined(__SSE4_2__)
void _shemi_phoenician_to_phoenician_string_sse4_2(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
);
#endif

HFN_ void shemi_phoenician_to_phoenician_string(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	// For small inputs (< 16) prefer SSE 4.2, for larger prefer AVX2
#if defined(__AVX2__) && defined(__SSE4_2__)
	if (len < 16)
		_shemi_phoenician_to_phoenician_string_sse4_2(ptr, len, from, to);
	else
		_shemi_phoenician_to_phoenician_string_avx2(ptr, len, from, to);
#elif defined(__AVX2__)
	if (len < 16)
		_shemi_phoenician_to_phoenician_string_scalar(ptr, len, from, to);
	else
		_shemi_phoenician_to_phoenician_string_avx2(ptr, len, from, to);
#elif defined(__SSE4_2__)
	_shemi_phoenician_to_phoenician_string_avx2(ptr, len, from, to);
#else
	_shemi_phoenician_to_phoenician_string_scalar(ptr, len, from, to);
#endif
}

#ifdef __cplusplus
}
#endif
