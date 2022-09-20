#pragma once
#include <iostream>
#include "key.h"

//#define DEBUG_FUNC

using namespace std;

static size_t it = 0;

// Encryption
void Encryption(Key key, string& message);
void AES(uint32_t* block, Key key);
void AddRoundKey(uint32_t* block, Key key, int round);
void SubBytes(uint32_t* block);
void ShiftRows(uint32_t* block);
void MixColumns(uint32_t* block);

// Decryption
void Decryption(Key key, string& message);
void InvAES(uint32_t* block, Key key);
void InvShiftRows(uint32_t* block);
void InvSubBytes(uint32_t* block);
void InvMixColumns(uint32_t* block);