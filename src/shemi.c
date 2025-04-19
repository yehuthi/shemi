#include "shemi.h"
#include <stdint.h>

static const char32_t HEBREW_NORMALIZE_TABLE[] = {
	0 , 1 , 2 , 3 , 4 , 5 , 6 , 7 , 8 , 9 , 10, 10, 11, 12, 12, 13, 13, 14,
	15, 16, 16, 17, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30
};

static bool char_in_range(char32_t low, char32_t high, char32_t value) {
	return low <= value && value <= high;
}

bool shemi_block_hebrew(char32_t c) {
	return char_in_range(0x0590, 0x05FF, c);
}

bool shemi_block_hebrew_alphabet(char32_t c) {
	return char_in_range(0x05D0, 0x05EA, c);
}

bool shemi_block_hebrew_cantillation(char32_t c) {
	return char_in_range(0x0591, 0x05AF, c);
}

bool shemi_block_phoenician(char32_t c) {
	return char_in_range(0x10900, 0x1091F, c);
}

bool shemi_block_phoenician_alphabet(char32_t c) {
	return char_in_range(0x10900, 0x10915, c);
}

bool shemi_block_phoenician_number(char32_t c) {
	return char_in_range(0x10916, 0x1091B, c);
}

bool shemi_block_aramaic(char32_t c) {
	return char_in_range(0x10840, 0x1085F, c);
}

bool shemi_block_aramaic_alphabet(char32_t c) {
	return char_in_range(0x10840, 0x10855, c);
}

bool shemi_block_aramaic_number(char32_t c) {
	return char_in_range(0x10858, 0x1085F, c);
}

bool shemi_block_samaritan(char32_t c) {
	return char_in_range(0x0800, 0x083F, c);
}

bool shemi_block_samaritan_alphabet(char32_t c) {
	return char_in_range(0x0800, 0x0815, c);
}

char32_t shemi_hebrew_to_phoenician_unchecked(char32_t c) {
	return 0x10900 + HEBREW_NORMALIZE_TABLE[c - 0x05DA];
}

char32_t shemi_hebrew_to_phoenician(char32_t c) {
	return shemi_block_hebrew_alphabet(c) ?
		shemi_hebrew_to_phoenician_unchecked(c) : c;
}

static const char32_t HEBREW_SOFIYOT[] = {
	0x05DA, 0x05DD, 0x05DF, 0x05E3, 0x05E5
};
static const char32_t HEBREW_NOT_SOFIYOT[] = {
	0x05DB, 0x05DE, 0x05E0, 0x05E4, 0x05E6
};
const uint8_t HEBREW_SOFIYOT_COUNT =
	sizeof(HEBREW_SOFIYOT) / sizeof(HEBREW_SOFIYOT[0]);

bool shemi_hebrew_sofit(char32_t c) {
	for (uint8_t i = 0; i < HEBREW_SOFIYOT_COUNT; i++)
		if (HEBREW_NOT_SOFIYOT[i] == c) return true;
	return false;
}

char32_t shemi_hebrew_to_sofit(char32_t c) {
	for (uint8_t i = 0; i < HEBREW_SOFIYOT_COUNT; i++)
		if (HEBREW_NOT_SOFIYOT[i] == c) return c - 1;
	return c;
}

char32_t shemi_hebrew_to_not_sofit(char32_t c) {
	for (uint8_t i = 0; i < HEBREW_SOFIYOT_COUNT; i++)
		if (HEBREW_SOFIYOT[i] == c) return c + 1;
	return c;
}
