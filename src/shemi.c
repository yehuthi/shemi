#include "shemi.h"
#include <emmintrin.h>

void _shemi_phoenician_to_phoenician_string_scalar(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	for (size_t i = 0; i < len; i++)
		ptr[i] = shemi_phoenician_to_phoenician(ptr[i], from, to);
}

#if defined(__AVX2__)
void _shemi_phoenician_to_phoenician_string_avx2(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	const __m256i alef_p = _mm256_set1_epi32(from - 1);
	const __m256i taw_p = _mm256_set1_epi32(from + 22 + 1);
	const __m256i unicode_offset = _mm256_set1_epi32(to - from);

	size_t i = 0;
	for (; i + 7 < len; i += 8) {
		const __m256i chr = _mm256_loadu_si256((const __m256i*)&ptr[i]);
		const __m256i mask = _mm256_and_si256(
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

	_shemi_phoenician_to_phoenician_string_scalar(&ptr[i], len - i, from, to);
}
#endif

#if defined(__SSE4_2__)
void _shemi_phoenician_to_phoenician_string_sse4_2(
	char32_t *const ptr, size_t len, char32_t from, char32_t to
) {
	const __m128i alef_p = _mm_set1_epi32(from - 1);
	const __m128i taw_p = _mm_set1_epi32(from + 22 + 1);
	const __m128i unicode_offset = _mm_set1_epi32(to - from);

	size_t i = 0;
	for (; i + 3 < len; i += 4) {
		const __m128i chr = _mm_loadu_si128((const __m128i*)&ptr[i]);
		const __m128i mask = _mm_and_si128(
			_mm_cmpgt_epi32(chr, alef_p),
			_mm_cmpgt_epi32(taw_p, chr)
		);
		const __m128i converted = _mm_add_epi32(unicode_offset, chr);
		const __m128i masked = _mm_or_si128(
			_mm_and_si128(converted, mask),
			_mm_andnot_si128(mask, chr)
		);
		_mm_storeu_si128((void*)&ptr[i], masked);
	}

	_shemi_phoenician_to_phoenician_string_scalar(&ptr[i], len - i, from, to);
}
#endif
