#include <iostream>
#include "CFBoard.h"

int main() {
	//testing rollback
	CFBoard board = CFBoard();

	std::cout << board.getRepr();

	//make some dummy moves
	board.movePiece(48, 40);

	
	std::cout << board.getRepr();

	board.movePiece(49, 41);

	std::cout << board.getRepr();

}