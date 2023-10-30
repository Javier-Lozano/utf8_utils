#include "utf8_utils.h"

/// @brief Get the length of a string taking UTF-8 encoded characters into consideration
/// @param str Pointer to the string
/// @return Count of characters from the string
size_t utf8_strlen(const char *str)
{
	size_t len = 0;
	unsigned char ch = 0;

	while(ch = *(str++))
	{
		if ((ch > 0xC1 && ch < 0xF5) || ch < 0x80) { len++; }
	}

	/*
	for (size_t i = 0; str[i]; i++)
	{
		ch = str[i];
		if ((ch > 0xC1 && ch < 0xF5) || ch < 0x80) { len++; }
	}
	*/
	
	return len;
}

/// @brief Count the number of consecutive set (1) bits from a byte. The count starts from the most significant bit then the byte is shifted left until reaching an unset (0) bit. This function helps to identify the given byte as an ASCII character, an UTF-8 starting byte or an UTF-8 continuation byte.
/// @param ch An unsigned character byte to analyze
/// @return Number of consecutive set bits
uint8_t utf8_count_bits(unsigned char ch)
{
	unsigned char bytes = 0;
	while(ch & 0x80)
	{
		bytes++;
		ch = (ch << 1);
	}

	// for (unsigned char check = 0x80; c & check; check = check >> 1, bytes++);
	
	return bytes;
}

/// @brief Maps every character from a string to an 32 bits unsigned integer array
/// @param str Pointer to a string
/// @param map_length A pointer to fill in the map's length
/// @return An array of integers representing every valid UTF-8 character on the string
uint32_t *utf8_map_to_uint32(const char *str, size_t *map_length)
{
	unsigned char ch = 0;
	uint32_t *map = NULL;
	size_t map_len = 0;
	size_t map_index = 0;

	if (str == NULL) { return 0; }

	map_len = utf8_strlen(str);
	map = (uint32_t*)calloc(map_len, sizeof(uint32_t));

	while(ch = *(str++))
	{
		if (ch > 0xC1 && ch < 0xF5)
		{
			map[map_index] = ch;
			while( ((ch = *(str++)) & 0xC0) == 0x80)
			{
				map[map_index] = (map[map_index] << 8) | ch;
			}
			map_index++;
			str--;
		}
		else if (ch < 0x80)	// ASCII character
		{
			map[map_index] = ch;
			map_index++;
		}
	}

	/*
	for (size_t i = 0; str[i]; i++)
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
	*/

	if (map_length != NULL) { *map_length = map_len; }

	return map;
}

/// @brief Retrieve a character from a string and store it inside an array


void utf8_retrieve_character(const char *str, size_t index, char *dest, int dest_length)
{
	const uint8_t * s = NULL;

	if (str == NULL) { return; }
	if (dest_length < 5) { return; }

	s = (const uint8_t*)str;

	if (s[index] < 0x80)
	{
		//if (dest_length < 2) { return; }
		dest[0] = str[index];
		dest[1] = '\0';
	}
	else if (s[index] > 0x80 && s[index] < 0xC0)
	{
		for (size_t i = index; i >= 0 ; i--)
		{
			if (s[i] > 0xC1 && s[i] < 0xF5)
			{
				uint8_t bytes = utf8_count_bits(s[i]);
				for (size_t b = 0; b < bytes; b++)
				{
					dest[b] = s[i+b];
				}
				dest[bytes] = '\0';
				break;
			}
		}
	}
	else if (s[index] > 0xC1 && s[index] < 0xF5)
	{
		uint8_t bytes = utf8_count_bits(s[index]);
		for (size_t b = 0; b < bytes; b++)
		{
			dest[b] = s[index+b];
		}
		dest[bytes] = '\0';
	}
}
