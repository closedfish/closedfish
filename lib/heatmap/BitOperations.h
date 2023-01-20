#pragma once

#ifndef CHAR_BIT
#define CHAR_BIT (sizeof(char))
#endif

#ifdef _MSC_VER
#include <immintrin.h>
#include <nmmintrin.h>
#define __builtin_popcountll _mm_popcnt_u64
#define __builtin_ctzll _tzcnt_u64
#define __builtin_clzll _lzcnt_u64
#endif

// !!! HOTFIX (Sirawit): This should actually reuse the code from CFBoard, we
// will change later when we have time.

#include <vector>
#include <cstdint>

uint64_t reverseBit(uint64_t v);

std::vector<int> bitSetPositions(uint64_t board);

bool isBitSet(const uint64_t &board, const int &tile);