#pragma once
#include <iostream>
#include "key.h"

using namespace std;

uint32_t CombineWord(uint8_t word[4]);
uint32_t CombineWord(string word);
uint8_t* BreakWord(uint32_t word);
string BlockToString(uint32_t* block);