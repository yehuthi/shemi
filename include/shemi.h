#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <memory.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>
#include <immintrin.h>

// Header function macros
// _HFN  = constexpr-able function
// _HFN_ = non constexpr-able function
// _HFD = _HFN (constexpr-able) data
#ifdef __cplusplus
	#if __cpp_constexpr >= 200704L
		#define _HFN constexpr
		#define _HFN_ inline
		#define _HFD constexpr
	#else
		#define _HFN inline
		#define _HFN_ _HFN
		#define _HFD static const
	#endif
#else
	#define _HFN static inline
	#define _HFN_ _HFN
	#define _HFD static const
#endif

#ifdef __cplusplus
#define _DEFVAL(A) = A
#else
#define _DEFVAL(A)
#endif

/** @defgroup scripts Scripts */
/** @defgroup scripts_phnx Phoenician Scripts
 * @ingroup scripts
 */

/// @brief Phoenician script.
/// @ingroup scripts scripts_phnx
#define SHEMI_PHNX (0x10900)
/// @brief Hebrew script.
/// @ingroup scripts
#define SHEMI_HEBR (0x005D0)
/// @brief Imperial-Aramaic script.
/// @ingroup scripts scripts_phnx
#define SHEMI_ARMI (0x10840)
/// @brief Samaritan script.
/// @ingroup scripts scripts_phnx
#define SHEMI_SAMR (0x00800)

_HFN bool _shemi_char_in_range(char32_t low, char32_t high, char32_t value) {
	return low <= value && value <= high;
}
#define _BLOCK_FN(NAME, START, END)                  \
	_HFN bool shemi_block_##NAME(char32_t c) {       \
		return _shemi_char_in_range(START, END, c); \
	}
_BLOCK_FN(hebrew             , 0x00590, 0x005FF)
_BLOCK_FN(hebrew_alphabet    , 0x005D0, 0x005EA)
_BLOCK_FN(hebrew_cantillation, 0x00591, 0x005AF)
_BLOCK_FN(phoenician         , 0x10900, 0x1091F)
_BLOCK_FN(phoenician_alphabet, 0x10900, 0x10915)
_BLOCK_FN(phoenician_number  , 0x10916, 0x1091B)
_BLOCK_FN(aramaic            , 0x10840, 0x1085F)
_BLOCK_FN(aramaic_alphabet   , 0x10840, 0x10855)
_BLOCK_FN(aramaic_number     , 0x10858, 0x1085F)
_BLOCK_FN(samaritan          , 0x00800, 0x0083F)
_BLOCK_FN(samaritan_alphabet , 0x00800, 0x00815)
#undef _BLOCK_FN

_HFD char32_t _SHEMI_HEBREW_NORMALIZE_TABLE[] = {
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 10, 11, 12, 12, 13, 13, 14,
	15, 16, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30,
	0
};
_HFD size_t _SHEMI_HEBREW_NORMALIZE_TABLE_LEN =
	(sizeof(_SHEMI_HEBREW_NORMALIZE_TABLE) /
	sizeof(_SHEMI_HEBREW_NORMALIZE_TABLE[0]))
	- 1;

/// @brief Converts a Hebrew character to a Phoenician-family script character.
/// @param c A Hebrew alphabet character.
/// @param target The target Phoenician-family script (see @ref scripts_phnx).
/// @warning The given character `c` MUST be within the Hebrew alphabet range
/// `[0x05D0-0x05EA]`.
/// @see `shemi_hebrew_to_phoenician`.
/// @return The equivalent Phoenician-equivalent for `c`.
_HFN char32_t shemi_hebrew_to_phoenician_unchecked(char32_t c, char32_t target) {
	return target + _SHEMI_HEBREW_NORMALIZE_TABLE[c - 0x05D0];
}

/// @brief Converts a Hebrew character to a Phoenician-family script character.
/// @param c A character.
/// @param target The target Phoenician-family script (see @ref scripts_phnx).
/// @see `shemi_hebrew_to_phoenician_unchecked`.
/// @return The equivalent Phoenician-equivalent for `c`, or `c` if the given
/// character is not Hebrew.
_HFN char32_t shemi_hebrew_to_phoenician(char32_t c, char32_t target) {
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
/// @param c A character.
/// @return Whether `c` is sofit.
_HFN bool shemi_hebrew_is_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_NOT_SOFIYOT[i] == c) return true;
	return false;
}

/// @brief Converts a Hebrew character to its sofit (final) variant.
/// @param c A character.
/// @return The sofit variant of `c`, or `c` if not applicable.
_HFN char32_t shemi_hebrew_to_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_NOT_SOFIYOT[i] == c) return c - 1;
	return c;
}

/// @brief Converts a Hebrew character to its non-sofit (non-final) variant.
/// @param c A character.
/// @return The non-sofit variant of `c`, or `c` if not applicable.
_HFN char32_t shemi_hebrew_to_not_sofit(char32_t c) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_SOFIYOT_COUNT; i++)
		if (_SHEMI_HEBREW_SOFIYOT[i] == c) return c + 1;
	return c;
}

/// @brief Converts a Phoenician-family character from one Phoenician-family
/// script to another.
/// @param c The Phoenician-family character.
/// @param from The Phoenician-family (@ref scripts_phnx) script `c` belongs to.
/// @param to The Phoenician-family script (@ref scripts_phnx) to convert to.
/// @warning The given character `c` MUST be within the `from`'s alphabet
/// character range.
/// @see `shemi_phoenician_to_phoenician`,
/// `shemi_phoenician_to_phoenician_string`
/// @return The equivalent for `c` in the `to` script family.
_HFN char32_t shemi_phoenician_to_phoenician_unchecked(
	char32_t c, char32_t from, char32_t to
) {
	return c + (to - from);
}

/// @brief Converts a Phoenician-family character from one Phoenician-family
/// script to another.
/// @param c The Phoenician-family character.
/// @param from The Phoenician-family (@ref scripts_phnx) script `c` belongs to.
/// @param to The Phoenician-family script (@ref scripts_phnx) to convert to.
/// @see `shemi_phoenician_to_phoenician_unchecked`,
/// `shemi_phoenician_to_phoenician_string`.
/// @return The equivalent for `c` in the `to` script family, or `c` if `c` does
/// not belong to `from`.
_HFN char32_t shemi_phoenician_to_phoenician(
	char32_t c, char32_t from, char32_t to
) {
	if (c < from || c > (from + 21)) return c;
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

/// @brief Converts a `char32_t` string from one Phoenician-script family to
/// another.
/// @param ptr The string.
/// @param len The string length.
/// @param from The Phoenician-family (@ref scripts_phnx) script `c` belongs to.
/// @param to The Phoenician-family script (@ref scripts_phnx) to convert to.
_HFN_ void shemi_phoenician_to_phoenician_string(
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

_HFN char32_t _shemi_phoenician_to_hebrew_from_table_index_unchecked(
	uint8_t offset,
	char32_t c,
	bool not_sofiyot
) {
	for (uint8_t i = 0; i < _SHEMI_HEBREW_NORMALIZE_TABLE_LEN; i++) {
		if (offset == _SHEMI_HEBREW_NORMALIZE_TABLE[i]) {
			const bool sofitable =
				_SHEMI_HEBREW_NORMALIZE_TABLE[i] ==
				_SHEMI_HEBREW_NORMALIZE_TABLE[i + 1];
			return 0x05D0 + i + (sofitable & not_sofiyot);
		}
	}
	return c;
}

/// @brief Converts a Phoenician-family character to its Hebrew equivalent.
/// @param c The Phoenician character.
/// @param from The Phoenician-family (@ref scripts_phnx) script `c` belongs to.
/// @param not_sofiyot Whether to use the sofit variant of the letter or not.
/// @warning The given character `c` MUST be within the `from`'s alphabet
/// character range.
/// @see `shemi_phoenician_to_hebrew`.
/// @return The equivalent Hebrew character for `c`, or `c` if not applicable.
_HFN char32_t shemi_phoenician_to_hebrew_unchecked(
	char32_t c,
	char32_t from,
	bool not_sofiyot _DEFVAL(true)
) {
	const uint8_t offset = (uint8_t)(c - from);
	return _shemi_phoenician_to_hebrew_from_table_index_unchecked(
		offset, c, not_sofiyot
	);
}

/// @brief Converts a Phoenician-family character to its Hebrew equivalent.
/// @param c The Phoenician character.
/// @param from The Phoenician-family (@ref scripts_phnx) script `c` belongs to.
/// @param not_sofiyot Whether to use the sofit variant of the letter or not.
/// @see `shemi_phoenician_to_hebrew_unchecked`.
/// @return The equivalent Hebrew character for `c`, or `c` if not applicable.
_HFN char32_t shemi_phoenician_to_hebrew(
	char32_t c,
	char32_t from,
	bool not_sofiyot _DEFVAL(true)
) {
	if (c <= from || c > (from + 21)) return c;
	const uint8_t offset = (uint8_t)(c - from);
	return _shemi_phoenician_to_hebrew_from_table_index_unchecked(
		offset, c, not_sofiyot
	);
}

#ifdef __cplusplus
}
#endif
