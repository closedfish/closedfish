#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include "../../lib/board_implementation/CFBoard.cpp"

bool validSquare(int i, int j) {
	return i >= 0 && i < 8 && j >= 0 && j < 8;
}
std::vector<int> tileToPos(int tile) {
	return {tile/8, tile%8};
}

int posToTile(std::vector<int> pos) {
	return pos[0] * 8 + pos[1];
}

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

std::vector<int> tilesSetFromBoard(uint64_t board) {
	std::vector<int> tiles;
	int cur = 0;
	while (board != 0) {
		int rightmost_tile = __builtin_ctz(board);
		cur += rightmost_tile+1;
		tiles.push_back(cur-1);
		board >>= rightmost_tile+1;
	}
	return tiles;
}

// std::vector<std::vector<int>> posSetFromBoard(const uint64_t &board) {
//	 std::vector<std::vector<int>> pos;
//	 std::vector<int> tiles = tilesSetFromBoard(board);
//	 for (auto x: tiles) {
//		 pos.push_back(posToTile(tiles));
//	 }
//	 return pos;
// }

bool hasPawnOn(const uint64_t& pawn_board, const std::vector<int> &pos) {
	return (pawn_board>>posToTile(pos))&1;
}

bool squareNotAttackedByPawn(const uint64_t& opponent_pawn_board, 
							const int& row, const int &col) {
	return row == 7 || ((col == 0 || !hasPawnOn(opponent_pawn_board, {row+1, col-1}))
				&& (col == 7 || !hasPawnOn(opponent_pawn_board, {row+1, col+1})));
}

void displayPawnBoard(const uint64_t& pawnBoard) { // for testing only
	uint64_t one = 1;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << ((pawnBoard>>(i*8+j))&one);
		}
		std::cout << '\n';
	}
}

void addHeatMapPieceProtect(int &i, int &j, int (&heat_map)[8][8], const char &piece,
							int &no_pieces, uint64_t &opponent_pawn_board, int (&pawn_height)[8]) {
	if (no_pieces == 0) return;
	std::vector<std::vector<int>> next_squares;
	if (piece == 'P') {
		if (validSquare(i-1, j-1)) heat_map[i-1][j-1]++;
		if (validSquare(i-1, j+1)) heat_map[i-1][j+1]++;
	} else if (piece == 'N') {
		int di[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
		int dj[8] = {2, 1, -1, -2, -2, -1, 1, 2};
		for (int k = 0; k < 8; k++) {
			next_squares.push_back({i+di[k], j+dj[k]});
		}
	} else if (piece == 'B') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			next_squares.push_back({i+k, j+k}); // same first diagonal
			next_squares.push_back({i+k, j-k}); // same second diagonal
		}
	} else if (piece == 'R') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			next_squares.push_back({i, j+k}); // same rank
			next_squares.push_back({i+k, j}); // same file
		}
	} else if (piece == 'Q') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			next_squares.push_back({i+k, j+k}); // same first diagonal
			next_squares.push_back({i+k, j-k}); // same second diagonal
			next_squares.push_back({i, j+k}); // same rank
			next_squares.push_back({i+k, j}); // same file
		}
	} else if (piece == 'K') {

	}
	for (auto p: next_squares) {
		int next_i = p[0], next_j = p[1];
		if (validSquare(next_i, next_j) && squareNotAttackedByPawn(opponent_pawn_board, next_i, next_j) && next_i < pawn_height[next_j]) {
			heat_map[next_i][next_j] += no_pieces;
		}
	}
}

void addHeatMap(CFBoard& board, int (&heat_map)[8][8], const uint64_t &weak_pawns) {
	uint64_t mask_row[8], mask_col[8]; // 1 for one column or one row, 0 otherwise
	mask_row[0] = (1LL<<8)-1;
	mask_col[0] = 1LL + (1LL<<8) + (1LL<<16) + (1LL<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
	for (int i = 1; i < 8; i++) {
		mask_row[i] = mask_row[i-1]<<8;
		mask_col[i] = mask_col[i-1]<<1;
	}

	bool current_turn = 1; // 0: white, 1: black
	uint64_t pawn_board = board.getPieceColorBitBoard(0|current_turn),
	knight_board = board.getPieceColorBitBoard(2|current_turn),
	bishop_board = board.getPieceColorBitBoard(4|current_turn),
	rook_board = board.getPieceColorBitBoard(6|current_turn),
	queen_board = board.getPieceColorBitBoard(8|current_turn),
	king_board = board.getPieceColorBitBoard(10|current_turn);
	if (!current_turn) { // switch orientation from white to black for easier code
		pawn_board = reverseBit(pawn_board);
		knight_board = reverseBit(knight_board);
		bishop_board = reverseBit(bishop_board);
		rook_board = reverseBit(rook_board);
		queen_board = reverseBit(queen_board);
		king_board = reverseBit(king_board);
	}

	// Counting pieces
	int no_knights = __builtin_popcountll(knight_board),
		no_rooks = __builtin_popcountll(rook_board),
		no_queens = __builtin_popcountll(queen_board);
	// Count how many light and dark squared bishop
	int no_bishops_color[2] = {0, 0};
	std::vector<int> bishop_tiles = tilesSetFromBoard(bishop_board);
	for (int tile: bishop_tiles) {
		int row = tile/8, col = tile%8;
		no_bishops_color[(row+col)%2]++;
	}

	int pawn_height[8] = {8, 8, 8, 8, 8, 8, 8, 8};
	// displayPawnBoard(pawn_board);
	for (int i = 0; i < 8; i++) { 
		uint64_t pawn_row = (pawn_board & mask_row[i]) >> (8*i);
		if (!pawn_row) continue;
		for (int j = 0; j < 8; j++) {
			if ((pawn_row >> j)&1 && pawn_height[j] == 8) {
				pawn_height[j] = i;
			}
		}
	}

	// Calculating pawn heights of each column
	std::vector<int> open_file;
	for (int j = 0; j < 8; j++) {
		if (pawn_height[j] == 8) {
			open_file.push_back(j);
		}
	}

	// Number of free rows below the lowest pawn
	int free_rows = (*std::min_element(pawn_height, pawn_height+8)); // number of rows below the lowest pawn
	// std::cout << free_rows << '\n';
	
	uint64_t opponent_pawn_board = board.getPieceColorBitBoard(0|(!current_turn));
	if (!current_turn) {
		opponent_pawn_board = reverseBit(opponent_pawn_board);
	}
	if (open_file.size() > 0) { // there exists open files
		for (int j: open_file) {
			int max_pawn_height = std::min(j > 0 ? pawn_height[j-1] : 8, j < 7 ? pawn_height[j+1] : 8);

			// Rooks and Queens move to open file
			for (int i = 0; i < max_pawn_height; i++) {
				if (squareNotAttackedByPawn(opponent_pawn_board, i, j)) // no opposing pawns
					heat_map[i][j] += 2 * (no_rooks + no_queens); // weighted since they are more important
			}

			// Knights move to square near open file, defending the nearby pawns and pieces

			// defend open file
			for (int i = 0; i < max_pawn_height; i++) {
				addHeatMapPieceProtect(i, j, heat_map, 'N', no_knights, opponent_pawn_board, pawn_height);
			}
			// protect left pawn
			if (j >= 1 && validSquare(pawn_height[j-1], j)) {
				addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'N', no_knights, opponent_pawn_board, pawn_height);
			}
			// protect right pawn
			if (j <= 6 && validSquare(pawn_height[j+1], j)) {
				addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'N', no_knights, opponent_pawn_board, pawn_height);
			}

			// Bishops also move to square near open file, defending the nearby pawns and pieces

			//defend open file
			for (int i = 0; i < max_pawn_height; i++) {
				bool bishop_color = (i+j)%2;
				addHeatMapPieceProtect(i, j, heat_map, 'B', no_bishops_color[bishop_color], opponent_pawn_board, pawn_height);
			}
			// protect left pawn
			if (j >= 1 && validSquare(pawn_height[j-1], j)) {
				addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
										opponent_pawn_board, pawn_height);
			}
			// protect right pawn
			if (j <= 6 && validSquare(pawn_height[j+1], j)) {
				addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
										opponent_pawn_board, pawn_height);
			}
		}
	} else { // No open files
		std::cout << free_rows << '\n';
		if (free_rows >= 2) { // 2 free rows, knights, rooks and queens are essentially able to go anywhere
			// To be added
			// Rooks and Queens
			std::vector<int> weak_pawns_file;
			for (int i = 0; i < 8; i++) {
				if (weak_pawns & (1ll<<i)) { // ith bit is set
					weak_pawns_file.push_back(i);
				}
			}

			for (int j: weak_pawns_file) {
				int max_pawn_height = std::min(j > 0 ? pawn_height[j-1] : 8, j < 7 ? pawn_height[j+1] : 8);

				// Rooks and Queens move to behind the weak pawn
				for (int i = 0; i < max_pawn_height; i++) {
					if (squareNotAttackedByPawn(opponent_pawn_board, i, j)) // no opposing pawns
						heat_map[i][j] += std::min(pawn_height[j]+1, no_rooks + no_queens); // weighted since they are more important
				}

				int opponent_pawn_height = pawn_height[j]+1;
				// Knights move to square near weak pawn, attacking it
				addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'N', no_knights, opponent_pawn_board, pawn_height);

				// Bishops attack weak pawns or defend current pawns, depends on the color of the bishop
				// attacking
				addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'B', no_bishops_color[(j+opponent_pawn_height)%2], opponent_pawn_board, pawn_height);
				// protect left pawn
				if (j >= 1 && validSquare(pawn_height[j-1], j)) {
					addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
											opponent_pawn_board, pawn_height);
				}
				// protect right pawn
				if (j <= 6 && validSquare(pawn_height[j+1], j)) {
					addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
											opponent_pawn_board, pawn_height);
				}

			} 
		} else {

			// To be added
		}
	}
}

int main() {
	int heat_map[8][8];
	memset(heat_map, 0, sizeof heat_map);

	CFBoard board;
	// board.fromFEN("rnbqkbnr/8/5p1p/1p2pPpP/pP1pP1P1/P2P4/8/RNBQKBNR w KQkq - 0 1"); // open file
	board.fromFEN("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/P1B1QB1R/3K3R w - - 0 1"); // no open files, >= 2 free rows
	uint64_t weak_pawns = 1ll; // placeholder for finished weak pawns implementation
	// std::cout << board.getRepr();
	addHeatMap(board, heat_map, weak_pawns);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << heat_map[i][j] << ' ';
		}
		std::cout << '\n';
	}
}