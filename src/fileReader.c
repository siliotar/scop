#include "fileReader.h"

char	*readFile(const char* filePath)
{
	FILE* f = fopen(filePath, "rb");
	fseek(f, 0, SEEK_END);
	unsigned int fsize = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* string = malloc(fsize + 1);
	if (string == NULL)
	{
		printf("Malloc error\n");
		return NULL;
	}
	fread(string, 1, fsize, f);
	fclose(f);

	string[fsize] = 0;
	return string;
}