#include <iostream>
#include "../heatmap/Heatmap.h"
#include "../board_implementation/CFBoard.h"

//-------------THIS FILE IS GOING TO BE USED FOR TESTING EVERY SINGLE IMPORTANT LIB FILE --------------------

int main() {
	CFBoard board = CFBoard();
	board.fromFEN("rnbqkbnr/8/5p1p/1p2pPpP/pP1pP1P1/P2P4/8/RNBQKBNR w KQkq - 0 1");

	int heatmap[6][8][8];
	memset(heatmap, 0, sizeof heatmap);

	uint64_t weakPawns = 1ll; // placeholder for finished weak pawns implementation

	Heatmap::addHeatMap(board, heatmap, weakPawns);


	//print the heatmap to debug 
	for (int i = 0; i < 6; i++) {
		std::cout << "Testing for piece type = " << i << std::endl;

		for (int j = 0; j < 8; j++) {
			for (int k = 0; k < 8; k++) {
				std::cout << heatmap[i][j][k];
			}

			std::cout << std::endl;
		}
	}

}