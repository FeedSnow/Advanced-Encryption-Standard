#include <fstream>
#include <bitset>
#include "block_ops.h"

uint32_t CombineWord(uint8_t word[4])
{
	uint32_t result = word[0];
	for (int i = 1; i < 4; i++)
	{
		result <<= 8;
		result += word[i];
	}
	return result;
}

uint32_t CombineWord(string word)
{
	if (word.size() != 4)
	{
		cout << "Invalid word length!" << endl;
		return 0;
	}

	uint32_t result = word[0];
	for (int i = 1; i < 4; i++)
	{
		result <<= 8;
		result += word[i];
	}
	return result;
}

uint8_t* BreakWord(uint32_t word)
{
	uint8_t* result = new uint8_t[4];
	for (int i = 0; i < 4; i++)
		result[i] = uint8_t(word >> (8 * (3 - i)));
	return result;
}

string BlockToString(uint32_t* block)
{
	string s = "";
	uint8_t** brokenBlock = new uint8_t * [4];
	for (int i = 0; i < 4; i++)
		brokenBlock[i] = BreakWord(block[i]);

	for (int j = 0; j < 4; j++)
		for (int i = 0; i < 4; i++)
			s += brokenBlock[i][j];

	return s;
}