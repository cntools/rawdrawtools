#define FONT_CREATION_TOOL
#define CNFG_IMPLEMENTATION
#define FONT_DEVELOPER

#include "oldfont.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

size_t const num_chars = 256;
unsigned char **charArray;

unsigned char *charData; //Array of points for this character

int segmentLength = 0; //how many points since the last segment cut

#ifdef OLDFONT
void LoadFont()
{

	int characterToLoad;

	for (characterToLoad = 0; characterToLoad < 256; characterToLoad++)
	{
		int characterDataBytes = 1;
		charArray[characterToLoad] = malloc(8 * sizeof(char)); //allocating 8 points (bytes/characters) per character
		int index = RawdrawFontCharMap[characterToLoad];

		if (index == 65535)
		{

			memset(&(charArray[characterToLoad][0]), 0x89, 1);
			memset(&(charArray[characterToLoad][1]), 0x80, 1);
		}
		else
		{

			unsigned char *characterData = &RawdrawFontCharData[index];

			unsigned char *characterDestination = charArray[characterToLoad];

			int c = -1;
			memset(&characterDestination[0], 0x40, 1);
			characterDataBytes++;
			do
			{
				c++;
				characterDataBytes++;
				//If we have used 8 or more bytes and the current count is a power of 2 (8,16,32...) double the amount of memory for this array
				if (characterDataBytes >= 8 && ((characterDataBytes & (characterDataBytes - 1)) == 0))
				{

					unsigned char *tmp = (unsigned char *)realloc(charArray[characterToLoad], sizeof(char) * (characterDataBytes << 1));
					if (tmp != NULL && tmp != charArray[characterToLoad])
					{
						charArray[characterToLoad] = tmp;
						characterDestination = tmp;
					}
				}
				short x = (short)(((characterData[c] >> 4) & 0x07));
				short y = (short)(characterData[c] & 0x07);
				short segmentBreak= (short)(characterData[c] & 0x08)>>3;

				//printf("%d\n",y);	
				char point=(characterData[c] & 0x80) + (segmentBreak << 6) + (x << 3) + (y);
				//printf("%d\n", (int)(characterData[c] & 0x80) + (segmentBreak << 6) + (x << 3) + y);
				memset(&characterDestination[c + 1], point, 1);

			} while (c == 0 || (characterData[c] & 0x80) != 0x80);
		}
	}
	printf("Font Loaded\n");
}

#else
void LoadFont()
{

	for (int i = 0; i < num_chars; i++)
	{
		charArray[i] = malloc(8 * sizeof(char)); //allocating 8 points (bytes/characters) per character
		memset(charArray[i], 0x80, 1);
	}
	printf("No Font Loaded\n");
}
#endif // !FONTINIT

void SaveFont(char *filename)
{
	FILE *f = fopen(filename, "wb");
	int characterIndex[256];
	unsigned char *AllCharacterData = malloc(sizeof(char) * 8);
	memset(&AllCharacterData[0], 0x80, 1);

	int totalPoints = 0;

	int characterToSave;

	for (characterToSave = 0; characterToSave < 256; characterToSave++)
	{

		unsigned char *characterData = charArray[characterToSave];
		int c = -1;

		//If the character has lines from previously, count the amount of points and segments
		if ((characterData[1] & 0x80) != 0x80)
		{
			characterIndex[characterToSave] = totalPoints + 1;

			do
			{

				c++;
				totalPoints++;
				if (totalPoints >= 7 && ((totalPoints & (totalPoints - 1)) == 0)) //If we have used 8 or more bytes and the current count is a power of 2 (8,16,32...) double the amount of memory for this array
				{
					//	printf("char %c too big, allocating more\n", characterToSave);
					unsigned char *tmp = (unsigned char *)realloc(AllCharacterData, sizeof(char) * (totalPoints << 1));
					if (tmp != NULL && tmp != AllCharacterData)
					{							//If the memory was reallocated properly and we get a new pointer
						AllCharacterData = tmp; //Get the new Address
					}
				}
				AllCharacterData[totalPoints] = characterData[c];

				//printf("%x\n", characterData[c]);

			} while (c == 0 || (characterData[c] & 0x80) != 0x80);
		}
		else if (characterData[1] & 0x40 == 0x40)
		{
			totalPoints++;
			characterIndex[characterToSave] = totalPoints;
			AllCharacterData[totalPoints] = characterData[0];
			totalPoints++;
			AllCharacterData[totalPoints] = characterData[1];
		}
		else
		{
			characterIndex[characterToSave] = 0;
		}
	}
	fprintf(f, "%s\n%s\n", "#ifndef FONTINIT", "#define FONTINIT");
	fprintf(f, "int CharIndex[] = {\n\t");

	for (int i = 0; i < 256; i++)
		fprintf(f, "%4d,%s", characterIndex[i], (((i + 1) % 16) == 0) ? "\n\t" : " ");

	fprintf(f, "};\n\n");

	fprintf(f, "unsigned char FontData[] = {\n\t");
	for (int i = 0; i <= totalPoints; i++)
		fprintf(f, "0x%02x,%s", AllCharacterData[i], (((i + 1) % 16) == 0) ? "\n\t" : " ");
	fprintf(f, "};\n\n");

	fprintf(f, "%s\n", "#endif");
	fclose(f);
}

int main()
{
	charArray = malloc(sizeof(char *) * num_chars); //Allocating the memory for the array of pointers that'll store every character's info
	LoadFont();
	printf("Loaded\n");
	SaveFont("output.c");
	printf("saved\n");
	return 0;
}
