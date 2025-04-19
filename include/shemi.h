#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <uchar.h>
#include <stdbool.h>

bool shemi_block_hebrew(char32_t c);
bool shemi_block_hebrew_alphabet(char32_t c);
bool shemi_block_hebrew_cantillation(char32_t c);
bool shemi_block_phoenician(char32_t c);
bool shemi_block_phoenician_alphabet(char32_t c);
bool shemi_block_phoenician_number(char32_t c);
bool shemi_block_aramaic(char32_t c);
bool shemi_block_aramaic_alphabet(char32_t c);
bool shemi_block_aramaic_number(char32_t c);

char32_t shemi_hebrew_to_phoenician_unchecked(char32_t c);
char32_t shemi_hebrew_to_phoenician(char32_t c);
bool shemi_hebrew_sofit(char32_t c);
char32_t shemi_hebrew_to_sofit(char32_t c);
char32_t shemi_hebrew_to_not_sofit(char32_t c);

#ifdef __cplusplus
}
#endif