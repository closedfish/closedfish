uint64_t reverseBit(uint64_t v) {
	//credit to https://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
	uint64_t r = v; // r will be reversed bits of v; first get LSB of v
	int s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end

	for (v >>= 1; v; v >>= 1)
	{   
		r <<= 1;
		r |= v & 1;
		s--;
	}
	r <<= s; // shift when v's highest bits are zero
	return r;
}

std::vector<int> bitSetPositions(uint64_t board) {
	std::vector<int> positions;
	while (board != 0) {
		positions.push_back(__builtin_ctzll(board));
		uint64_t lb = board & -board;    // get lowest bit
		board ^= lb;      // remove lowest bit from tmp
	}
	return positions;
}

bool isBitSet(const uint64_t& board, const int &tile) {
	return board&(1ll<<tile);
}