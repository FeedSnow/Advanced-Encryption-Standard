#include <fstream>
#include "key.h"
#include "block_ops.h"
#include "lookup.h"

// ####################################
// #                                  #
// #               MAIN               #
// #                                  #
// ####################################

Key::Key(string key)
{
	N = key.size() / 8;
	original_key = new uint32_t[N];

	for (int i = 0; i < N; i++)
		original_key[i] = CombineWord(key.substr(i * 4, 4));

	switch (N)
	{
	case 6:
		R += 2;
	case 5:
		R += 2;
	case 4:
		R += 11;
		break;
	}

	expanded_key = new uint32_t[4 * R];

	for (int i = 0; i < 4 * R; i++)
	{
		if (i < N)
			expanded_key[i] = original_key[i];
		else if (i % N == 0)
			expanded_key[i] = expanded_key[i - N] ^ SubWord(RotWord(expanded_key[i - 1])) ^ rcon(i / N);
		else if (N > 6 && i % N == 4)
			expanded_key[i] = expanded_key[i - N] ^ SubWord(expanded_key[i - 1]);
		else
			expanded_key[i] = expanded_key[i - N] ^ expanded_key[i - 1];
	}
}

Key::Key()
{
	N = 0;
	R = 0;
	original_key = nullptr;
	expanded_key = nullptr;
}

uint32_t* Key::RoundKey(int i)
{
	uint32_t* rk = new uint32_t[4];
	for (int j = 0; j < 4; j++)
		rk[j] = expanded_key[4 * i + j];
	return rk;
}

Key EmptyKey()
{
	return Key();
}



// ####################################
// #                                  #
// #              INPUT               #
// #                                  #
// ####################################

Key InitKey()
{
	int option;
	do
	{
		system("cls");
		cout << "1 - Input key from keyboard" << endl;
		cout << "2 - Input key from file" << endl;
		cout << "> ";
		cin >> option;
	} while (option != 1 && option != 2);

	if (option == 1)
		return InputKey();

	return ReadKey();
}

ifstream KeyFile()
{
	string filename;
	ifstream is;
	system("cls");
	do
	{
		cout << "Input name of the key file:" << endl;
		cout << "> ";
		cin >> filename;

		is.open(filename);
		if (!is)
			cout << "Couldn't open file " + filename << endl << endl;

	} while (!is);

	return is;
}

Key ReadKey()
{
	ifstream is = KeyFile();

	size_t it = 0;
	string key = "";
	is >> key;

	if (CheckKey(key))
	{
		CutKey(key);
		return Key(key);
	}

	cout << "Invalid key!" << endl;
	return EmptyKey();
}

Key InputKey()
{
	string key = "";

	system("cls");
	cout << "Input key in hexadecimal system (32, 48 or 64 digits):" << endl;
	cout << "> ";
	cin >> key;
	cout << endl;

	while (!CheckKey(key))
	{
		cout << "Invalid key!" << endl;
		cout << "Input key:" << endl;
		cout << "> ";
		cin >> key;
		cout << endl;
	}

	CutKey(key);
	return Key(key);
}



// ####################################
// #                                  #
// #            AUXILIARY             # 
// #                                  #
// ####################################

uint32_t RotWord(uint32_t word)
{
	uint32_t tmp = word >> 24;
	word <<= 8;
	word += tmp;
	return word;
}

uint32_t InvRotWord(uint32_t word)
{
	uint32_t tmp = word << 24;
	word >>= 8;
	word += tmp;
	return word;
}

uint32_t SubWord(uint32_t word)
{
	uint8_t* broken = BreakWord(word);

	for (int i = 0; i < 4; i++)
		broken[i] = Lookup::sbox[broken[i]];

	return CombineWord(broken);
}

uint32_t InvSubWord(uint32_t word)
{
	uint8_t* broken = BreakWord(word);

	for (int i = 0; i < 4; i++)
		broken[i] = Lookup::inv_sbox[broken[i]];

	return CombineWord(broken);
}

uint32_t rcon(int i)
{
	uint8_t word[4] = { Lookup::rc[i], 0x0, 0x0, 0x0 };
	return CombineWord(word);
}

bool CheckKey(string key)
{
	if (key.size() < 32)
		return false;

	for (int i = 0; i < key.size(); i++)
		if (!isxdigit(key[i]))
			return false;

	return true;
}

void CutKey(string& key)
{
	if (key.size() > 32)
	{
		if (key.size() > 48)
		{
			if (key.size() > 64)
			{
				key = key.substr(0, 64);
				return;
			}

			key = key.substr(0, 48);
			return;
		}

		key = key.substr(0, 32);
		return;
	}
}