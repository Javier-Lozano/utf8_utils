#include "utf8_utils.h"

/// @brief Get the length of a UTF-8 compliant string
/// @param str Pointer to a string
/// @return Count of characters from a UTF-8 string
size_t utf8_strlen(const char *str)
{
	const uint8_t *s = NULL;
	size_t len = 0;

	if (str == NULL) { return 0; }
	
	s = (const uint8_t *)str;

	for (size_t i = 0; s[i]; i++)
	{
		if ((s[i] > 0xC1 && s[i] < 0xF5) || s[i] < 0x80) { len++; }
	}
	
	return len;

}

/// @brief Count the number of consecutive "1" bits from the most significant to the least significant position until finding a "0" bit or running through the entire byte.
/// @param s The byte to run through
/// @return Number of consecutive ON bits
uint8_t utf8_count_bits(const uint8_t s)
{
	uint8_t bytes = 0;
	for (uint8_t check = 0x80; s & check; check = check >> 1, bytes++);

	return bytes;
}

/// @brief Maps an UTF-8 compliant string to an uint32 array
/// @param str Pointer to a string
/// @param map_length A pointer to store the map length
/// @return An array of integers representing an UTF-8 compliant string
uint32_t *utf8_map_to_uint32(const char *str, size_t *map_length)
{
	const uint8_t *s = NULL;
	uint32_t *map = NULL;
	size_t map_len = 0;
	size_t map_index = 0;

	if (str == NULL) { return 0; }

	s = (const uint8_t *)str;

	map_len = utf8_strlen(str);
	map = (uint32_t*)calloc(map_len, sizeof(uint32_t));

	for (size_t i = 0; s[i]; i++)
	{
		if (s[i] > 0xC1 && s[i] < 0xF5) // UTF-8 starting byte
		{
			uint8_t bytes = utf8_count_bits(s[i]);
			for (uint8_t b = 0; b < bytes; b++)
			{
				map[map_index] = map[map_index] << 8 | s[i + b];
			}
			map_index++;
		}
		else if (s[i] < 0x80)	// ASCII character
		{
			map[map_index] = s[i];
			map_index++;
		}
	}

	if (map_length != NULL) { *map_length = map_len; }

	return map;
}