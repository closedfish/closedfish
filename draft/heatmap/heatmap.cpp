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

bool squareNotAttackedByPawn(const uint64_t& opponentPawnBoard, 
							const int& row, const int &col) {
	return row == 7 || ((col == 0 || !isBitSet(opponentPawnBoard, posToTile({row+1, col-1})))
				&& (col == 7 || !isBitSet(opponentPawnBoard, posToTile({row+1, col+1}))));
}

void displayPawnBoard(const uint64_t& pawnBoard) { // for testing only
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << ((pawnBoard>>(i*8+j))&1ll);
		}
		std::cout << '\n';
	}
}

void addHeatMapPieceProtect(const int &i, const int &j, int (&heatMap)[8][8], const char &piece,
							const int &noPieces, const uint64_t &opponentPawnBoard, int (&pawnHeight)[8], const int &coefficient = 1) {
	if (noPieces == 0) return;
	std::vector<std::vector<int>> nextSquares;
	if (piece == 'P') {
		if (validSquare(i-1, j-1)) heatMap[i-1][j-1]++;
		if (validSquare(i-1, j+1)) heatMap[i-1][j+1]++;
	} else if (piece == 'N') {
		int di[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
		int dj[8] = {2, 1, -1, -2, -2, -1, 1, 2};
		for (int k = 0; k < 8; k++) {
			nextSquares.push_back({i+di[k], j+dj[k]});
		}
	} else if (piece == 'B') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			nextSquares.push_back({i+k, j+k}); // same first diagonal
			nextSquares.push_back({i+k, j-k}); // same second diagonal
		}
	} else if (piece == 'R') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			nextSquares.push_back({i, j+k}); // same rank
			nextSquares.push_back({i+k, j}); // same file
		}
	} else if (piece == 'Q') {
		for (int k = -7; k <= 7; k++) {
			if (k == 0) continue;
			nextSquares.push_back({i+k, j+k}); // same first diagonal
			nextSquares.push_back({i+k, j-k}); // same second diagonal
			nextSquares.push_back({i, j+k}); // same rank
			nextSquares.push_back({i+k, j}); // same file
		}
	} else if (piece == 'K') {
		int di[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
		int dj[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
		for (int k = 0; k < 8; k++) {
			nextSquares.push_back({i+di[k], j+dj[k]});
		}
	}
	for (auto p: nextSquares) {
		int next_i = p[0], next_j = p[1];
		if (validSquare(next_i, next_j) && squareNotAttackedByPawn(opponentPawnBoard, next_i, next_j) && next_i < pawnHeight[next_j]) {
			heatMap[next_i][next_j] += noPieces * coefficient;
		}
	}
}

void addHeatMap(CFBoard& board, int (&heatMap)[8][8], const uint64_t &weakPawns) {
	uint64_t maskRow[8], maskCol[8]; // 1 for one column or one row, 0 otherwise
	maskRow[0] = (1LL<<8)-1;
	maskCol[0] = 1LL + (1LL<<8) + (1LL<<16) + (1LL<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
	for (int i = 1; i < 8; i++) {
		maskRow[i] = maskRow[i-1]<<8;
		maskCol[i] = maskCol[i-1]<<1;
	}

	bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
	uint64_t pawnBoard = board.getPieceColorBitBoard(0|currentTurn),
	knightBoard = board.getPieceColorBitBoard(2|currentTurn),
	bishopBoard = board.getPieceColorBitBoard(4|currentTurn),
	rookBoard = board.getPieceColorBitBoard(6|currentTurn),
	queenBoard = board.getPieceColorBitBoard(8|currentTurn),
	kingBoard = board.getPieceColorBitBoard(10|currentTurn);
	if (!currentTurn) { // switch orientation from white to black for easier code
		pawnBoard = reverseBit(pawnBoard);
		knightBoard = reverseBit(knightBoard);
		bishopBoard = reverseBit(bishopBoard);
		rookBoard = reverseBit(rookBoard);
		queenBoard = reverseBit(queenBoard);
		kingBoard = reverseBit(kingBoard);
	}

	// Counting pieces
	int noKnights = __builtin_popcountll(knightBoard),
		noRooks = __builtin_popcountll(rookBoard),
		noQueens = __builtin_popcountll(queenBoard);

	// Count how many light and dark squared bishop
	int noBishopsColor[2] = {0, 0};
	std::vector<int> bishop_tiles = bitSetPositions(bishopBoard);
	for (int tile: bishop_tiles) {
		int row = tile/8, col = tile%8;
		noBishopsColor[(row+col)%2]++;
	}

	int pawnHeight[8] = {8, 8, 8, 8, 8, 8, 8, 8};
	// displayPawnBoard(pawnBoard);
	for (int i = 0; i < 8; i++) { 
		uint64_t pawnRow = (pawnBoard & maskRow[i]) >> (8*i);
		if (!pawnRow) continue;
		for (int j = 0; j < 8; j++) {
			if ((pawnRow >> j)&1 && pawnHeight[j] == 8) {
				pawnHeight[j] = i;
			}
		}
	}

	// Calculating pawn heights of each column
	std::vector<int> openFiles;
	for (int j = 0; j < 8; j++) {
		if (pawnHeight[j] == 8) {
			openFiles.push_back(j);
		}
	}

	// Number of free rows below the lowest pawn
	int free_rows = (*std::min_element(pawnHeight, pawnHeight+8)); // number of rows below the lowest pawn
	// std::cout << free_rows << '\n';
	
	uint64_t opponentPawnBoard = board.getPieceColorBitBoard(0|(!currentTurn));
	if (!currentTurn) {
		opponentPawnBoard = reverseBit(opponentPawnBoard);
	}
	if (openFiles.size() > 0) { // there exists open files
		for (int j: openFiles) {
			int maxPawnHeight = std::min(j > 0 ? pawnHeight[j-1] : 8, j < 7 ? pawnHeight[j+1] : 8);

			// Rooks and Queens move to open file, priotizing staying on the lowest rank
			for (int i = 0; i < maxPawnHeight; i++) {
				if (squareNotAttackedByPawn(opponentPawnBoard, i, j)) // no opposing pawns
					heatMap[i][j] += std::max(pawnHeight[j]+1 - i + noRooks + noQueens, 0); // weighted since they are more important
			}

			// Knights move to square near open file, defending the nearby pawns and pieces

			// defend open file
			for (int i = 0; i < maxPawnHeight; i++) {
				addHeatMapPieceProtect(i, j, heatMap, 'N', noKnights, opponentPawnBoard, pawnHeight);
			}
			// protect left pawn
			if (j >= 1 && validSquare(pawnHeight[j-1], j)) {
				addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'N', noKnights, opponentPawnBoard, pawnHeight);
			}
			// protect right pawn
			if (j <= 6 && validSquare(pawnHeight[j+1], j)) {
				addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'N', noKnights, opponentPawnBoard, pawnHeight);
			}

			// Bishops also move to square near open file, defending the nearby pawns and pieces

			//defend open file
			for (int i = 0; i < maxPawnHeight; i++) {
				bool bishop_color = (i+j)%2;
				addHeatMapPieceProtect(i, j, heatMap, 'B', noBishopsColor[bishop_color], opponentPawnBoard, pawnHeight);
			}
			// protect left pawn
			if (j >= 1 && validSquare(pawnHeight[j-1], j)) {
				addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'B', noBishopsColor[(pawnHeight[j-1]+j)%2],
										opponentPawnBoard, pawnHeight);
			}
			// protect right pawn
			if (j <= 6 && validSquare(pawnHeight[j+1], j)) {
				addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'B', noBishopsColor[(pawnHeight[j-1]+j)%2],
										opponentPawnBoard, pawnHeight);
			}
		}
	} else { // No open files
		// std::cout << free_rows << '\n';
		std::vector<int> weakPawnFiles;
		for (int i = 0; i < 8; i++) {
			if (weakPawns & (1ll<<i)) { // ith bit is set
				weakPawnFiles.push_back(i);
			}
		}
		if (free_rows >= 2) { // 2 free rows, knights, rooks and queens are essentially able to go anywhere
			// To be adjusted
			for (int j: weakPawnFiles) {
				int maxPawnHeight = std::min(j > 0 ? pawnHeight[j-1] : 8, j < 7 ? pawnHeight[j+1] : 8);

				// Rooks move to behind the weak pawn, priotizing staying on the lowest rank
				for (int i = 0; i < maxPawnHeight; i++) {
					if (squareNotAttackedByPawn(opponentPawnBoard, i, j)) // no opposing pawns
						heatMap[i][j] += std::max(pawnHeight[j]+1 - i + noRooks, 0);
				}

				int opponentPawnHeight = pawnHeight[j]+1;
				// Queens attack weak pawns from the diagonals
				addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'B', noQueens, opponentPawnBoard, pawnHeight);

				// Knights move to square near weak pawn, attacking it
				addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'N', noKnights, opponentPawnBoard, pawnHeight);

				// Bishops attack weak pawns or defend current pawns, depends on the color of the bishop
				// attacking
				addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'B', noBishopsColor[(j+opponentPawnHeight)%2], opponentPawnBoard, pawnHeight);
				// protect left pawn
				if (j >= 1 && validSquare(pawnHeight[j-1], j)) {
					addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'B', noBishopsColor[(pawnHeight[j-1]+j)%2],
											opponentPawnBoard, pawnHeight);
				}
				// protect right pawn
				if (j <= 6 && validSquare(pawnHeight[j+1], j)) {
					addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'B', noBishopsColor[(pawnHeight[j-1]+j)%2],
											opponentPawnBoard, pawnHeight);
				}

				// King moves to near weak pawns, protect nearby pawns if opponent does not have many materials left on the board.
				//														otherwise, stay as far as possible
				int threshold = 15; // to be adjusted
				int opponentMaterial = board.getMaterialCount(!currentTurn);
				if (opponentMaterial < threshold) {
					// protect left pawn
					if (j >= 1 && validSquare(pawnHeight[j-1], j)) {
						addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'K', 1, opponentPawnBoard, pawnHeight);
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawnHeight[j+1], j)) {
						addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'K', 1, opponentPawnBoard, pawnHeight);
					}
				} else {
					// stay in the left if weak pawn in the right
					if (j >= 4) {
						addHeatMapPieceProtect(0, 0, heatMap, 'K', 1, opponentPawnBoard, pawnHeight);
					}
					// stay in the right otherwise
					else {
						addHeatMapPieceProtect(0, 7, heatMap, 'K', 1, opponentPawnBoard, pawnHeight);
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
				if (pawnHeight[i] == 1 && i != 0) {
					numArea[i] = 2*cur+1;
					cur++;
				} else {
					numArea[i] = 2*cur;
				}
			}
			int countArea = numArea[7];
			int distArea[8][8];
			for (int j1 = 0; j1 < 8; j1++) {
				for (int j2 = 0; j2 < 8; j2++) {
					distArea[j1][j2] = countArea - abs(numArea[j1] - numArea[j2]); // gets smaller if two areas are far apart
				}
			}
			
			std::vector<std::vector<int>> knightPos = posSetFromBoard(knightBoard),
										bishopPos = posSetFromBoard(bishopBoard), 
										rookPos = posSetFromBoard(rookBoard), 
										queenPos = posSetFromBoard(queenBoard), 
										kingPos = posSetFromBoard(kingBoard);
			// To be adjusted
			for (int j: weakPawnFiles) {
				int maxPawnHeight = std::min(j > 0 ? pawnHeight[j-1] : 8, j < 7 ? pawnHeight[j+1] : 8);

				// Rooks move to behind the weak pawn, priotizing staying on the lowest rank
				for (int i = 0; i < maxPawnHeight; i++) {
					if (squareNotAttackedByPawn(opponentPawnBoard, i, j)) { // no opposing pawns 
						for (std::vector<int> pos: rookPos)
							heatMap[i][j] += std::max(pawnHeight[j]+1 - i + distArea[j][pos[1]], 0);
					}
				}

				int opponentPawnHeight = pawnHeight[j]+1;
				// Queens attack weak pawns from the diagonals
				for (std::vector<int> pos: queenPos)
					addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'B', 1, opponentPawnBoard, pawnHeight, distArea[j][pos[1]]);

				// Knights move to square near weak pawn, attacking it
				for (std::vector<int> pos: knightPos)
					addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'N', 1, opponentPawnBoard, pawnHeight, distArea[j][pos[1]]);

				// Bishops attack weak pawns or defend current pawns, depends on the color of the bishop
				// attacking
				for (std::vector<int> pos: bishopPos) {
					bool bishopColor = (pos[0]+pos[1])%2;
					if ((j+opponentPawnHeight)%2 == bishopColor)
						addHeatMapPieceProtect(opponentPawnHeight, j, heatMap, 'B', 1,
											opponentPawnBoard, pawnHeight, distArea[j][pos[1]]);
					// protect left pawn
					if (j >= 1 && validSquare(pawnHeight[j-1], j) && (pawnHeight[j-1]+j)%2 == bishopColor) {
						addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'B', 1,
												opponentPawnBoard, pawnHeight, distArea[j][pos[1]]);
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawnHeight[j+1], j) && (pawnHeight[j-1]+j)%2 == bishopColor) {
						addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'B', 1,
												opponentPawnBoard, pawnHeight, distArea[j][pos[1]]);
					}
				}

				// King moves to near weak pawns, protect nearby pawns if opponent does not have many materials left on the board.
				//														otherwise, stay as far as possible
				int threshold = 15; // to be adjusted
				int opponentMaterial = board.getMaterialCount(!currentTurn);
				int kingCol = kingPos[0][1];
				if (opponentMaterial < threshold) {
					// protect left pawn
					if (j >= 1 && validSquare(pawnHeight[j-1], j)) {
						addHeatMapPieceProtect(pawnHeight[j-1], j, heatMap, 'K', 1, opponentPawnBoard, pawnHeight, distArea[j][kingCol]);
					}
					// protect right pawn
					if (j <= 6 && validSquare(pawnHeight[j+1], j)) {
						addHeatMapPieceProtect(pawnHeight[j+1], j, heatMap, 'K', 1, opponentPawnBoard, pawnHeight, distArea[j][kingCol]);
					}
				} else {
					// stay in the left if weak pawn in the right
					if (j >= 4) {
						addHeatMapPieceProtect(0, 0, heatMap, 'K', 1, opponentPawnBoard, pawnHeight, distArea[j][kingCol]);
					}
					// stay in the right otherwise
					else {
						addHeatMapPieceProtect(0, 7, heatMap, 'K', 1, opponentPawnBoard, pawnHeight, distArea[j][kingCol]);
					}
				}
			} 
		}
	}
	// Switch back to white's orientation if white's turn
	if (!currentTurn) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 8; j++) {
				std::swap(heatMap[i][j], heatMap[7-i][7-j]);
			}
		}
	}
}

int main() {
	int heatMap[8][8];
	memset(heatMap, 0, sizeof heatMap);

	CFBoard board;
	// board.fromFEN("rnbqkbnr/8/5p1p/1p2pPpP/pP1pP1P1/P2P4/8/RNBQKBNR w KQkq - 0 1"); // open file
	// board.fromFEN("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/2B1QB1R/3K3R w - - 0 1"); // no open files, >= 2 free rows
	// board.fromFEN("rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R w - - 0 1"); // no open files, 1 free rows, no chance of winning
	// board.fromFEN("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1"); // no open files, 1 free rows, some chance of winning
	board.fromFEN("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/1NBQRB1R w - - 0 1"); // same as above, rooks are in the same area
	uint64_t weakPawns = 1ll; // placeholder for finished weak pawns implementation
	// std::cout << board.getRepr();
	addHeatMap(board, heatMap, weakPawns);
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			std::cout << heatMap[i][j] << ' ';
		}
		std::cout << '\n';
	}
}