#include "BitOperations.h"

inline std::vector<int> bitSetPositions(uint64_t board) {
	std::vector<int> positions;
	while (board != 0) {
		positions.push_back(__builtin_ctzll(board));
		uint64_t lb = board & -board; // get lowest bit
		board ^= lb;									// remove lowest bit from tmp
	}
	return positions;
}
inline bool isBitSet(const uint64_t &board, const int &tile) {
	return board & (1ll << tile);
}