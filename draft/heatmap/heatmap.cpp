#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include "../../lib/board_implementation/CFBoard.cpp"
#include "BitOperations.h"

bool validSquare(int i, int j) {
	return i >= 0 && i < 8 && j >= 0 && j < 8;
}
std::vector<int> tileToPos(int tile) {
	return {tile/8, tile%8};
}

int posToTile(std::vector<int> pos) {
	return pos[0] * 8 + pos[1];
}

std::vector<std::vector<int>> posSetFromBoard(const uint64_t &board) {
	std::vector<std::vector<int>> pos;
	std::vector<int> tiles = bitSetPositions(board);
	for (auto x: tiles) {
		pos.push_back(tileToPos(x));
	}
	return pos;
}

bool squareNotAttackedByPawn(const uint64_t& opponent_pawn_board, 
							const int& row, const int &col) {
	return row == 7 || ((col == 0 || !isBitSet(opponent_pawn_board, posToTile({row+1, col-1})))
				&& (col == 7 || !isBitSet(opponent_pawn_board, posToTile({row+1, col+1}))));
}

void displayPawnBoard(const uint64_t& pawnBoard) { // for testing only
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << ((pawnBoard>>(i*8+j))&1ll);
		}
		std::cout << '\n';
	}
}

void addHeatMapPieceProtect(const int &i, const int &j, int (&heat_map)[8][8], const char &piece,
							const int &no_pieces, const uint64_t &opponent_pawn_board, int (&pawn_height)[8], const int &coefficient = 1) {
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
		int di[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
		int dj[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
		for (int k = 0; k < 8; k++) {
			next_squares.push_back({i+di[k], j+dj[k]});
		}
	}
	for (auto p: next_squares) {
		int next_i = p[0], next_j = p[1];
		if (validSquare(next_i, next_j) && squareNotAttackedByPawn(opponent_pawn_board, next_i, next_j) && next_i < pawn_height[next_j]) {
			heat_map[next_i][next_j] += no_pieces * coefficient;
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

	bool current_turn = board.getCurrentPlayer(); // 0: white, 1: black
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
	std::vector<int> bishop_tiles = bitSetPositions(bishop_board);
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

			// Rooks and Queens move to open file, priotizing staying on the lowest rank
			for (int i = 0; i < max_pawn_height; i++) {
				if (squareNotAttackedByPawn(opponent_pawn_board, i, j)) // no opposing pawns
					heat_map[i][j] += std::max(pawn_height[j]+1 - i + no_rooks + no_queens, 0); // weighted since they are more important
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
		std::vector<int> weak_pawns_file;
		for (int i = 0; i < 8; i++) {
			if (weak_pawns & (1ll<<i)) { // ith bit is set
				weak_pawns_file.push_back(i);
			}
		}
		if (free_rows >= 2) { // 2 free rows, knights, rooks and queens are essentially able to go anywhere
			// To be adjusted
			for (int j: weak_pawns_file) {
				int max_pawn_height = std::min(j > 0 ? pawn_height[j-1] : 8, j < 7 ? pawn_height[j+1] : 8);

				// Rooks move to behind the weak pawn, priotizing staying on the lowest rank
				for (int i = 0; i < max_pawn_height; i++) {
					if (squareNotAttackedByPawn(opponent_pawn_board, i, j)) // no opposing pawns
						heat_map[i][j] += std::max(pawn_height[j]+1 - i + no_rooks, 0);
				}

				int opponent_pawn_height = pawn_height[j]+1;
				// Queens attack weak pawns from the diagonals
				addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'B', no_queens, opponent_pawn_board, pawn_height);

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

				// King moves to near weak pawns, protect nearby pawns if opponent does not have many materials left on the board.
				//														otherwise, stay as far as possible
				int threshold = 15; // to be adjusted
				int opponent_material = board.getMaterialCount(!current_turn);
				if (opponent_material < threshold) {
					// protect left pawn
					if (j >= 1 && validSquare(pawn_height[j-1], j)) {
						addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'K', 1, opponent_pawn_board, pawn_height);
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawn_height[j+1], j)) {
						addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'K', 1, opponent_pawn_board, pawn_height);
					}
				} else {
					// stay in the left if weak pawn in the right
					if (j >= 4) {
						addHeatMapPieceProtect(0, 0, heat_map, 'K', 1, opponent_pawn_board, pawn_height);
					}
					// stay in the right otherwise
					else {
						addHeatMapPieceProtect(0, 7, heat_map, 'K', 1, opponent_pawn_board, pawn_height);
					}
				}
			} 
		} else {
			// Define "area" the spaces that pieces are free to move, each area is separated by a pawn of height 1

			// Pieces are more limited to their own "area", hence smaller coefficients added to
			// squares outside of their "area" since it takes more turns to move them there.
			
			int numArea[8];
			int cur = 0;
			for (int i = 0; i < 8; i++) {
				if (pawn_height[i] == 1 && i != 0) {
					numArea[i] = 2*cur+1;
					cur++;
				} else {
					numArea[i] = 2*cur;
				}
			}
			
			std::vector<std::vector<int>> knightPos = posSetFromBoard(knight_board),
							bishopPos = posSetFromBoard(bishop_board), 
							rookPos = posSetFromBoard(rook_board), 
							queenPos = posSetFromBoard(queen_board), 
							kingPos = posSetFromBoard(king_board);
			// To be adjusted
			for (int j: weak_pawns_file) {
				int max_pawn_height = std::min(j > 0 ? pawn_height[j-1] : 8, j < 7 ? pawn_height[j+1] : 8);

				// Rooks move to behind the weak pawn, priotizing staying on the lowest rank
				for (int i = 0; i < max_pawn_height; i++) {
					if (squareNotAttackedByPawn(opponent_pawn_board, i, j)) { // no opposing pawns 
						for (std::vector<int> pos: rookPos)
							heat_map[i][j] += std::max(pawn_height[j]+1 - i + (8 - abs(numArea[j]-numArea[pos[1]])), 0);
					}
				}

				int opponent_pawn_height = pawn_height[j]+1;
				// Queens attack weak pawns from the diagonals
				for (std::vector<int> pos: queenPos)
					addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'B', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-numArea[pos[1]]));

				// Knights move to square near weak pawn, attacking it
				for (std::vector<int> pos: knightPos)
					addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'N', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-numArea[pos[1]]));

				// Bishops attack weak pawns or defend current pawns, depends on the color of the bishop
				// attacking
				for (std::vector<int> pos: bishopPos) {
					bool bishopColor = (pos[0]+pos[1])%2;
					if ((j+opponent_pawn_height)%2 == bishopColor)
						addHeatMapPieceProtect(opponent_pawn_height, j, heat_map, 'B', 1,
											opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-numArea[pos[1]]));
					// protect left pawn
					if (j >= 1 && validSquare(pawn_height[j-1], j) && (pawn_height[j-1]+j)%2 == bishopColor) {
						addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
												opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-numArea[pos[1]]));
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawn_height[j+1], j) && (pawn_height[j-1]+j)%2 == bishopColor) {
						addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'B', no_bishops_color[(pawn_height[j-1]+j)%2],
												opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-numArea[pos[1]]));
					}
				}

				// King moves to near weak pawns, protect nearby pawns if opponent does not have many materials left on the board.
				//														otherwise, stay as far as possible
				int threshold = 15; // to be adjusted
				int opponent_material = board.getMaterialCount(!current_turn);
				int kingArea = numArea[kingPos[0][1]];
				if (opponent_material < threshold) {
					// protect left pawn
					if (j >= 1 && validSquare(pawn_height[j-1], j)) {
						addHeatMapPieceProtect(pawn_height[j-1], j, heat_map, 'K', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-kingArea));
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawn_height[j+1], j)) {
						addHeatMapPieceProtect(pawn_height[j+1], j, heat_map, 'K', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-kingArea));
					}
				} else {
					// stay in the left if weak pawn in the right
					if (j >= 4) {
						addHeatMapPieceProtect(0, 0, heat_map, 'K', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-kingArea));
					}
					// stay in the right otherwise
					else {
						addHeatMapPieceProtect(0, 7, heat_map, 'K', 1, opponent_pawn_board, pawn_height, 8 - abs(numArea[j]-kingArea));
					}
				}
			} 
		}
	}
	// Switch back to white's orientation if white's turn
	if (!current_turn) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				std::swap(heat_map[i][j], heat_map[7-i][7-j]);
			}
		}
	}
}

int main() {
	int heat_map[8][8];
	memset(heat_map, 0, sizeof heat_map);

	CFBoard board;
	// board.fromFEN("rnbqkbnr/8/5p1p/1p2pPpP/pP1pP1P1/P2P4/8/RNBQKBNR w KQkq - 0 1"); // open file
	board.fromFEN("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/2B1QB1R/3K3R w - - 0 1"); // no open files, >= 2 free rows
	// board.fromFEN("rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R w - - 0 1"); // no open files, 1 free rows, no chance of winning
	// board.fromFEN("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1"); // no open files, 1 free rows, some chance of winning
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