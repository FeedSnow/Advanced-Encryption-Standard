#pragma once
#include <iostream>

using namespace std;

class Key
{
public:
	Key(string key);
	uint32_t K(int i) { return original_key[i]; }
	uint32_t W(int i) { return expanded_key[i]; }
	uint32_t* RoundKey(int i);
	int GetN() { return N; }
	int GetR() { return R; }
	friend Key EmptyKey();

private:
	int N;
	int R = 0;
	uint32_t* original_key;
	uint32_t* expanded_key;
	Key();
};

Key EmptyKey();

// Input
Key InitKey();
Key ReadKey();
Key InputKey();

// Auxiliary
uint32_t RotWord(uint32_t word);
uint32_t InvRotWord(uint32_t word);
uint32_t SubWord(uint32_t word);
uint32_t InvSubWord(uint32_t word);
uint32_t rcon(int i);
bool CheckKey(string key);
void CutKey(string& key);