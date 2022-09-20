#include <fstream>
#include <bitset>
#include "rijndael.h"
#include "lookup.h"
#include "block_ops.h"


// ####################################
// #                                  #
// #            ENCRYPTION            #
// #                                  #
// ####################################

void ReadBlock(uint32_t* block, string message)
{
	string text = message.substr(it, 16);
	it += 16;
	for (int i = text.size(); i < 16; i++)
		text += ' ';

	for (int i = 0; i < 4; i++)
	{
		uint8_t word[4] = { (uint8_t)text[i], (uint8_t)text[i + 4] , (uint8_t)text[i + 8] , (uint8_t)text[i + 12] };
		block[i] = CombineWord(word);
	}
}

void Encryption(Key key, string& message)
{
	string output = "";
	uint32_t* block = new uint32_t[4];

	while (it < message.size())
	{
		ReadBlock(block, message);
		AES(block, key);
		output += BlockToString(block);
	}

	cout << endl;
	cout << "---------------------------------" << endl;
	cout << endl;

	cout << "Encrypted text: " << endl;
	cout << output << endl;
	message = output;
	it = 0;
}

void AES(uint32_t* block, Key key)
{
	AddRoundKey(block, key, 0);
	for (int i = 1; i < 10; i++)
	{
		SubBytes(block);
		ShiftRows(block);
		MixColumns(block);
		AddRoundKey(block, key, i);
	}
	SubBytes(block);
	ShiftRows(block);
	AddRoundKey(block, key, 10);
}

void AddRoundKey(uint32_t* block, Key key, int round)
{
	uint32_t* roundKey = key.RoundKey(round);
	for (int i = 0; i < 4; i++)
		block[i] ^= roundKey[i];
}

void SubBytes(uint32_t* block)
{
	for (int i = 0; i < 4; i++)
		block[i] = SubWord(block[i]);
}

void ShiftRows(uint32_t* block)
{
	for (int i = 1; i < 4; i++)
		for (int j = 0; j < i; j++)
			block[i] = RotWord(block[i]);
}

void MixColumns(uint32_t* block)
{
	uint8_t** brokenBlock = new uint8_t * [4];
	for (int i = 0; i < 4; i++)
		brokenBlock[i] = BreakWord(block[i]);

	uint8_t* column = new uint8_t[4];
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
			column[i] = brokenBlock[i][j];

		brokenBlock[0][j] = Lookup::mult2[column[0]] ^ Lookup::mult3[column[1]] ^ column[2] ^ column[3];
		brokenBlock[1][j] = column[0] ^ Lookup::mult2[column[1]] ^ Lookup::mult3[column[2]] ^ column[3];
		brokenBlock[2][j] = column[0] ^ column[1] ^ Lookup::mult2[column[2]] ^ Lookup::mult3[column[3]];
		brokenBlock[3][j] = Lookup::mult3[column[0]] ^ column[1] ^ column[2] ^ Lookup::mult2[column[3]];
	}

	for (int i = 0; i < 4; i++)
		block[i] = CombineWord(brokenBlock[i]);
}



// ####################################
// #                                  #
// #            DECRYPTION            # 
// #                                  #
// ####################################

void Decryption(Key key, string& message)
{
	string output = "";
	uint32_t* block = new uint32_t[4];

	while (it < message.size())
	{
		ReadBlock(block, message);
		InvAES(block, key);
		output += BlockToString(block);
	}

	cout << endl;
	cout << "---------------------------------" << endl;
	cout << endl;

	cout << "Decrypted text: " << endl;
	cout << output << endl;
	message = output;
	it = 0;
}

void InvAES(uint32_t* block, Key key)
{
	AddRoundKey(block, key, 10);
	InvShiftRows(block);
	InvSubBytes(block);
	for (int i = 9; i > 0; i--)
	{
		AddRoundKey(block, key, i);
		InvMixColumns(block);
		InvShiftRows(block);
		InvSubBytes(block);
	}
	AddRoundKey(block, key, 0);
}

void InvShiftRows(uint32_t* block)
{
	for (int i = 1; i < 4; i++)
		for (int j = 0; j < i; j++)
			block[i] = InvRotWord(block[i]);
}

void InvSubBytes(uint32_t* block)
{
	for (int i = 0; i < 4; i++)
		block[i] = InvSubWord(block[i]);
}

void InvMixColumns(uint32_t* block)
{
	uint8_t** brokenBlock = new uint8_t * [4];
	for (int i = 0; i < 4; i++)
		brokenBlock[i] = BreakWord(block[i]);

	uint8_t* column = new uint8_t[4];
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
			column[i] = brokenBlock[i][j];

		brokenBlock[0][j] = Lookup::mult14[column[0]] ^ Lookup::mult11[column[1]] ^ Lookup::mult13[column[2]] ^ Lookup::mult9[column[3]];
		brokenBlock[1][j] = Lookup::mult9[column[0]] ^ Lookup::mult14[column[1]] ^ Lookup::mult11[column[2]] ^ Lookup::mult13[column[3]];
		brokenBlock[2][j] = Lookup::mult13[column[0]] ^ Lookup::mult9[column[1]] ^ Lookup::mult14[column[2]] ^ Lookup::mult11[column[3]];
		brokenBlock[3][j] = Lookup::mult11[column[0]] ^ Lookup::mult13[column[1]] ^ Lookup::mult9[column[2]] ^ Lookup::mult14[column[3]];
	}

	for (int i = 0; i < 4; i++)
		block[i] = CombineWord(brokenBlock[i]);
}