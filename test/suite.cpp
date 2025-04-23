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
	{ SHEMI_PHNX, SHEMI_ARMI, SHEMI_SAMR };
static const char32_t* PHOENICIANS_FROM_HEBREW[] =
	{ PHNX_FROM_HEBREW, ARMI_FROM_HEBREW, SAMR_FROM_HEBREW };
static const char32_t* HEBREWS_FROM_PHOENICIAN[] =
	{ HEBR_FROM_PHNX_NO_SOFIYOT, HEBR_FROM_PHNX_SOFIYOT };
static const char* HEBREWS_NAMES[] =
	{ "Hebrew (No Sofiyot)", "Hebrew (Sofiyot)" };
static const bool HEBREWS_TAGS[] = { true, false };
const size_t HEBREWS_COUNT =
	sizeof(HEBREWS_FROM_PHOENICIAN) / sizeof(HEBREWS_FROM_PHOENICIAN[0]);

TEST(convert, hebrew_to_phoenician) {
	for (size_t s = 0; s < PHOENICIANS_COUNT; s++) {
		const char32_t *const expected_script = PHOENICIANS_FROM_HEBREW[s];
		for (size_t i = 0; i < HEBR_LEN; i++) {
			const char32_t original = HEBR[i];
			const char32_t actual =
				shemi_hebrew_to_phoenician(original, PHOENICIANS_TAGS[s]);
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

TEST(convert, hebrew_to_phoenician_foreign) {
	for (size_t i = 0; i < FOREIGN_LEN; i++) {
		const char32_t original = FOREIGN[i];
		const char32_t expected = original;
		const char32_t actual =
			shemi_hebrew_to_phoenician(original, SHEMI_PHNX);
		if (actual != expected) ADD_FAILURE() << std::hex <<
			"Conversion changed foreign character 0x" << original <<
			" to 0x" << actual;
		else SUCCEED() << std::hex << "0x" << original << " unchanged";
	}
}

TEST(convert, phoenician_to_phoenician) {
	for (size_t s = 0; s < PHOENICIANS_COUNT; s++) {
		for (size_t d = 0; d < PHOENICIANS_COUNT; d++) {
			for (size_t i = 0; i < PHNX_LEN; i++) {
				const char32_t original = PHOENICIANS[s][i];
				const char32_t expected = PHOENICIANS[d][i];
				const char32_t actual = shemi_phoenician_to_phoenician(
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

TEST(convert, phoenician_to_hebrew) {
	for (size_t s = 0; s < PHOENICIANS_COUNT; s++) {
		for (size_t d = 0; d < HEBREWS_COUNT; d++) {
			for (size_t i = 0; i < PHNX_LEN; i++) {
				const char32_t original = PHOENICIANS[s][i];
				const char32_t expected = HEBREWS_FROM_PHOENICIAN[d][i];
				const char32_t actual = shemi_phoenician_to_hebrew_unchecked(
					original, PHOENICIANS_TAGS[s], HEBREWS_TAGS[d]
				);
				if (actual != expected) ADD_FAILURE() << std::hex <<
					PHOENICIANS_NAMES[s] << " 0x" << original << " = " <<
					HEBREWS_NAMES[d] << " 0x" << expected << ", but got " <<
					"0x" << actual;
				else SUCCEED() << std::hex <<
					PHOENICIANS_NAMES[s] << " 0x" << original << " = " <<
					HEBREWS_NAMES[d] << " 0x" << expected;
			}
		}
	}
}

TEST(convert, phoenician_to_foreign) {
	for (size_t i = 0; i < FOREIGN_LEN; i++) {
		const char32_t original = FOREIGN[i];
		const char32_t expected = original;
		const char32_t actual =
			shemi_phoenician_to_phoenician(original, SHEMI_PHNX, SHEMI_ARMI);
		if (actual != expected) ADD_FAILURE() << std::hex <<
			"Conversion changed foreign character 0x" << original <<
			" to 0x" << actual;
		else SUCCEED() << std::hex << "0x" << original << " unchanged";
	}
}
#if defined(__AVX2__)
TEST(convert, phoenician_to_phoenician_avx2) {
	char32_t phnx[sizeof(PHNX)];
	memcpy(phnx, PHNX, sizeof(PHNX));
	_shemi_phoenician_to_phoenician_string_avx2(
		phnx, PHNX_LEN,
		SHEMI_PHNX, SHEMI_ARMI
	);
	for (size_t i = 0; i < PHNX_LEN; i++)
		EXPECT_EQ(phnx[i], ARMI[i]) << " at i = " << i;
}
#endif

#if defined(__SSE4_2__)
TEST(convert, phoenician_to_phoenician_sse4_2) {
	char32_t phnx[sizeof(PHNX)];
	memcpy(phnx, PHNX, sizeof(PHNX));
	_shemi_phoenician_to_phoenician_string_sse4_2(
		phnx, PHNX_LEN,
		SHEMI_PHNX, SHEMI_ARMI
	);
	for (size_t i = 0; i < PHNX_LEN; i++)
		EXPECT_EQ(phnx[i], ARMI[i]) << " at i = " << i;
}
#endif
