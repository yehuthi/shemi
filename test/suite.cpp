#include <gtest/gtest.h>
#include <stdint.h>
#include "shemi.h"

#define LETTER(NAME, HEBR, PHNX) \
	TEST(hebrew_convert_phoenician, NAME) { \
		const uint32_t actual = shemi_hebrew_convert(HEBR, SHEMI_PHOENICIAN);  \
		EXPECT_EQ(PHNX, actual) << std::hex << " expected: "                   \
			"f(0x" << HEBR << ") = 0x" << PHNX << " but got 0x" << actual; }
#include "hebr_to_phnx.inc.h"
#undef LETTER

TEST(hebrew_convert_non_hebrew, latin_a) {
	EXPECT_EQ('a', shemi_hebrew_convert('a', SHEMI_PHOENICIAN));
}

#define HEBR_ALEF 0x05D0
#define HEBR_TAW 0x05EA
#define PHNX_TAW 0x10915
#define ARAM_TAW 0x10855

TEST(hebrew_convert_aramaic, alef) {
	EXPECT_EQ(0x10840, shemi_hebrew_convert(HEBR_ALEF, SHEMI_ARAMAIC));
}

TEST(hebrew_convert_aramaic, taw) {
	EXPECT_EQ(0x10855, shemi_hebrew_convert(HEBR_TAW, SHEMI_ARAMAIC));
}

TEST(hebrew_convert_samaritan, alef) {
	EXPECT_EQ(0x0800, shemi_hebrew_convert(HEBR_ALEF, SHEMI_SAMARITAN));
}

TEST(hebrew_convert_samaritan, taw) {
	EXPECT_EQ(0x0815, shemi_hebrew_convert(HEBR_TAW, SHEMI_SAMARITAN));
}

TEST(phoenician_convert, phoenician_to_aramaic_taw) {
	EXPECT_EQ(
		ARAM_TAW,
		shemi_phoenician_convert_unchecked(
			PHNX_TAW,
			SHEMI_PHOENICIAN, SHEMI_ARAMAIC
		)
	);
}