#include <gtest/gtest.h>
#include <stdint.h>
#include "shemi.h"

#define LETTER(NAME, HEBR, PHNX) \
	TEST(hebrew_to_phoenician, NAME) { \
		const uint32_t actual = shemi_hebrew_to_phoenician(HEBR); \
		EXPECT_EQ(PHNX, actual) << std::hex << " expected: " \
			"f(0x" << HEBR << ") = 0x" << PHNX << " but got 0x" << actual; }
#include "hebr_to_phnx.inc.h"
#undef LETTER

TEST(hebrew_to_phoenician, latin_a) {
	EXPECT_EQ('a', shemi_hebrew_to_phoenician('a'));
}