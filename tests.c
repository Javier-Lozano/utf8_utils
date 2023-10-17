#include <stdio.h>
#include <string.h>
#include "utf8_utils.h"

int main()
{
	char *str = "AEIOUÁÉÍÓÚ";
	int len = strlen(str);
	int utf8_len = utf8_strlen(str);

	// utf8_strlen()
	//printf("\nUTF-8 string length\n");
	printf("\n\n");

	printf("String (%p): \"%s\"\n\tRaw string length: %d\n\tDecoded UTF-8 length: %d\n", str, str, strlen(str), utf8_strlen(str));

	// utf8_count_bits()
	//printf("\nCounting consecutive \"1\" bits from most significative to least significative.\n");
	printf("\n\n");

	for(size_t i = 0; str[i]; i++)
	{
		printf("Index: %d\t-> Value: %X\t-> Info: ", i, (uint8_t)str[i]);

		int bits = utf8_count_bits((uint8_t)str[i]);
		printf("(%d bits) ", bits);
		if (bits == 0) printf("ASCII\n");
		else if (bits == 1) printf("UTF-8 Data Byte\n");
		else if (bits > 1 && bits < 5) printf("UTF-8 Initial Byte\n");
		else printf("Not a character.\n");
	}

	// utf8_map_to_uint32()
	//printf("\nMap String to Uint32.\n");
	printf("\n\n");

	size_t map_length = utf8_strlen(str);
	uint32_t *map = utf8_map_to_uint32(str, NULL);
	for (size_t i = 0; i < map_length; i++)
	{
		printf("Index: %d -> Value: %X\n", i, map[i]);
	}

	if (map != NULL) { free(map); }
}