#ifndef _UTF8_UTILS_H_
#define _UTF8_UTILS_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

size_t utf8_strlen(const char *str);
uint8_t utf8_count_bits(const uint8_t c);
uint32_t *utf8_map_to_uint32(const char *str);
void utf8_retrieve_character(const char *str, size_t index, char *dest, int dest_length);

#endif // _UTF8_UTILS_H_
