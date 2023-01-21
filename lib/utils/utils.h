#ifdef _MSC_VER
#include <immintrin.h>
#include <nmmintrin.h>
#define __builtin_popcountll _mm_popcnt_u64
#define __builtin_ctzll _tzcnt_u64
#define __builtin_clzll _lzcnt_u64
#endif

#include "logger.h"
#include <CFBoard.h>
#include <stockfish/src/bitboard.h>
#include <stockfish/src/endgame.h>
#include <stockfish/src/position.h>
#include <stockfish/src/psqt.h>
#include <stockfish/src/search.h>
#include <stockfish/src/syzygy/tbprobe.h>
#include <stockfish/src/thread.h>
#include <stockfish/src/tt.h>
#include <stockfish/src/uci.h>

void convert_CFBoard_to_Stockfish_Position(CFBoard cfb,
																					 Stockfish::Position &pos,
																					 Stockfish::StateListPtr &states);
int parseAN(std::string str);
std::string toAN(int pos);