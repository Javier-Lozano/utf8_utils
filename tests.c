#include <stdio.h>
#include <string.h>
#include "utf8_utils.h"

int main()
{
	char *str = "AEIOUÁÉÍÓÚ\xFFá";
	char utf8_char[5] = {0};
	int len = strlen(str);
	int utf8_len = utf8_strlen(str);

	printf("String Length.\n\n");
	
	printf("String: \"%s\"\n", str);
	printf("\tRaw string length: %d\n", strlen(str));
	printf("\tUTF-8 string length: %d\n", utf8_strlen(str));

	printf("\n\n");
	printf("Character Analisis.\n\n");

	for(size_t i = 0; str[i]; i++)
	{
		printf("Index: %d:\n", i);
		printf("\tByte: 0x%X\n", (uint8_t)str[i]);

		int bits = utf8_count_bits((uint8_t)str[i]);
		switch (bits)
		{
		case 0:
			printf("\tCharacter: %c\n", str[i]);
			printf("\tInfo: ASCII character.\n");
			break;
		case 1:
			utf8_retrieve_character(str, i, utf8_char, 5);
			printf("\tCharacter: %s\n", utf8_char);
			printf("\tInfo: This is an UTF-8 continuation byte.\n");
			break;
		case 2:
		case 3:
		case 4:
			utf8_retrieve_character(str, i, utf8_char, 5);
			printf("\tCharacter: %s\n", utf8_char);
			printf("\tInfo: Staring byte of an UTF-8 character.\n");
			break;
		default:
			printf("\tThis byte doesn't represent a character!\n");
			break;
		}
	}

	printf("\n\n");
	printf("Mapping UTF-8 characters to uint32.\n\n");

	size_t map_length = utf8_strlen(str);
	uint32_t *map = utf8_map_to_uint32(str);
	for (size_t i = 0; i < map_length; i++)
	{
		printf("Index: %d -> Value: %X\n", i, map[i]);
	}

	if (map != NULL) { free(map); }
}
