#include "shemi.h"
#include <stdint.h>

static const char32_t hebrew_to_phoenician_table[] = {
	0x10900, 0x10901, 0x10902, 0x10903, 0x10904,
	0x10905, 0x10906, 0x10907, 0x10908, 0x10909,
	0x1090A, 0x1090A, /* kaf */
	0x1090B,
	0x1090C, 0x1090C, /* mem */
	0x1090D, 0x1090D, /* nun */
	0x1090E, 0x1090F,
	0x10910, 0x10910, /* pe */
	0x10911, 0x10911, /* sadi */
	0x10912, 0x10913, 0x10914, 0x10915
};

bool shemi_block_hebrew(char32_t c) {
	return 0x0590 <= c && c <= 0x05FF;
}

bool shemi_block_hebrew_alphabet(char32_t c) {
	return 0x05D0 <= c && c <= 0x05EA;
}

bool shemi_block_hebrew_cantillation(char32_t c) {
	return 0x0591 <= c && c <= 0x05AF;
}

bool shemi_block_phoenician(char32_t c) {
	return 0x10900 <= c && c <= 0x1091F;
}

bool shemi_block_phoenician_alphabet(char32_t c) {
	return 0x10900 <= c && c <= 0x10915;
}

bool shemi_block_phoenician_number(char32_t c) {
	return 0x10916 <= c && c <= 0x1091B;
}

bool shemi_block_aramaic(char32_t c) {
	return 0x10840 <= c && c <= 0x1085F;
}

bool shemi_block_aramaic_alphabet(char32_t c) {
	return 0x10840 <= c && c <= 0x10855;
}

bool shemi_block_aramaic_number(char32_t c) {
	return 0x10858 <= c && c <= 0x1085F;
}

bool shemi_block_samaritan(char32_t c) {
	return 0x0800 <= c && c <= 0x083F;
}

bool shemi_block_samaritan_alphabet(char32_t c) {
	return 0x0800 <= c && c <= 0x0815;
}

char32_t shemi_hebrew_to_phoenician_unchecked(char32_t c) {
	return hebrew_to_phoenician_table[c - 0x05DA];
}

char32_t shemi_hebrew_to_phoenician(char32_t c) {
	return shemi_block_hebrew_alphabet(c) ? shemi_hebrew_to_phoenician_unchecked(c) : c;
}

bool shemi_hebrew_sofit(char32_t c) {
	for (uint8_t i = 0; i < hebrew_sofiyot_count; i++)
		if (hebrew_not_sofiyot[i] == c) return true;
	return false;
}

static const char32_t hebrew_sofiyot[] = { 0x05DA, 0x05DD, 0x05DF, 0x05E3, 0x05E5 };
static const char32_t hebrew_not_sofiyot[] = { 0x5DB, 0x05DE, 0x05E0, 0x05E4, 0x05E6 };
const uint8_t hebrew_sofiyot_count = sizeof(hebrew_sofiyot) / sizeof(hebrew_sofiyot[0]);

char32_t shemi_hebrew_to_sofit(char32_t c) {
	for (uint8_t i = 0; i < hebrew_sofiyot_count; i++)
		if (hebrew_not_sofiyot[i] == c) return c - 1;
	return c;
}

char32_t shemi_hebrew_to_not_sofit(char32_t c) {
	for (uint8_t i = 0; i < hebrew_sofiyot_count; i++)
		if (hebrew_sofiyot[i] == c) return c + 1;
	return c;
}
