#include <gtest/gtest.h>
#include <stdint.h>
#include "shemi.h"

#define SUCCEED_REPORT 1
#ifdef SUCCEED_REPORT
#undef SUCCEED
class TestCout : public std::stringstream {
public:
    ~TestCout() {
        std::cout
			<< "\u001b[32m[  SUCCESS ] \u001b[33m"
			<< str()
			<< "\u001b[0m"
			<< std::endl << std::flush;
    }
};
#define SUCCEED() TestCout()
#endif

#include "test_data.h"
static const char32_t* PHOENICIANS[] = { PHNX, ARMI, SAMR };
static const size_t PHOENICIANS_COUNT =
	sizeof(PHOENICIANS) / sizeof(PHOENICIANS[0]);
static const char* PHOENICIANS_NAMES[] =
	{ "Phoenician", "Aramaic", "Samaritan" };
static const char32_t PHOENICIANS_TAGS[] =
	{ SHEMI_PHOENICIAN, SHEMI_ARAMAIC, SHEMI_SAMARITAN };
static const char32_t* PHOENICIANS_FROM_HEBREW[] =
	{ PHNX_FROM_HEBREW, ARMI_FROM_HEBREW, SAMR_FROM_HEBREW };

TEST(hebrew_convert, phoenicians) {
	for (size_t s = 0; s < PHOENICIANS_COUNT; s++) {
		const char32_t *const expected_script = PHOENICIANS_FROM_HEBREW[s];
		for (size_t i = 0; i < HEBR_LEN; i++) {
			const char32_t original = HEBR[i];
			const char32_t actual =
				shemi_hebrew_convert(original, PHOENICIANS_TAGS[s]);
			const char32_t expected = expected_script[i];
			if (actual != expected) ADD_FAILURE() << std::hex <<
				"Hebrew 0x" << original << " = " <<
				PHOENICIANS_NAMES[s] << " 0x" << expected <<
				" but conversion gave 0x" << actual;
			else SUCCEED() << std::hex <<
				"Hebrew 0x" << original << " = " <<
				PHOENICIANS_NAMES[s] << " 0x" << expected;
		}
	}
}

TEST(hebrew_convert, foreign) {
	for (size_t i = 0; i < FOREIGN_LEN; i++) {
		const char32_t original = FOREIGN[i];
		const char32_t expected = original;
		const char32_t actual =
			shemi_hebrew_convert(original, SHEMI_PHOENICIAN);
		if (actual != expected) ADD_FAILURE() << std::hex <<
			"Conversion changed foreign character 0x" << original <<
			" to 0x" << actual;
		else SUCCEED() << std::hex << "0x" << original << " unchanged";
	}
}

TEST(phoenician_convert, phoenicians) {
	for (size_t s = 0; s < PHOENICIANS_COUNT; s++) {
		for (size_t d = 0; d < PHOENICIANS_COUNT; d++) {
			for (size_t i = 0; i < PHNX_LEN; i++) {
				const char32_t original = PHOENICIANS[s][i];
				const char32_t expected = PHOENICIANS[d][i];
				const char32_t actual = shemi_phoenician_convert_unchecked(
					original, PHOENICIANS_TAGS[s], PHOENICIANS_TAGS[d]
				);
				if (actual != expected) ADD_FAILURE() << std::hex <<
					PHOENICIANS_NAMES[s] << " 0x" << original << " = " <<
					PHOENICIANS_NAMES[d] << " 0x" << expected << ", but got " <<
					"0x" << actual;
				else SUCCEED() << std::hex << 
					PHOENICIANS_NAMES[s] << " 0x" << original << " = " <<
					PHOENICIANS_NAMES[d] << " 0x" << expected;
			}
		}
	}
}