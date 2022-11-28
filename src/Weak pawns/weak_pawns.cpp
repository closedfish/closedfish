// Chess_AI.cpp : définit le point d'entrée de l'application.
//

#include "Chess_AI.h"
#include "CFBoard.cpp"

using namespace std;


int get_bit_from_pos(int i, int j) {
	return i*8 + j;
}

bool is_valid(int i, int j) {
	if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
		return true;
	}
	return false;
}

int nb_protecting_pawns(Position &pawn, CFBoard &board) {
//gets the number of currently protecting pawns
	int count = 0;
	if (pawn.j < 7) {
		if (pawn.i > 0) {
			if (board.get_bit_from_pos(Position(pawn.i - 1, pawn.j + 1)) == 1) {
				count++;
			}
		}
		if (pawn.i < 7) {
			if (board.get_bit_from_pos(Position(pawn.i + 1, pawn.j + 1)) == 1) {
				count++;
			}
		}
	}

	return count;
}

int nb_protecting_horses(CFBoard &board, bool &color, int& pawn_i, int& pawn_j){

	int count = 0;
	for (int i = -2; i < 3; i++) {
		for (int j = -2, j < 3; j++) {
			if (i != j && i != 0 && j!= 0){
				if (is_valid(pawn_i + i, pawn_j + j)){
					int tile = get_bit_from_pos(pawn_i + i, pawn_j + j);
					if (board.getBit('N', color, tile)) {
						count++;
					}
				}
				
			}
		}
	}
	return count;
}

bool is_row_free(CFBoard& board, bool &color, int& pawn_i, int& pawn_j, int& piece_i, int& piece_j) {
	//checks if the row if empty between the pawn to be protected and a rook or a queen

	int start = min(pawn_i, piece_i);
	int end = max(pawn_i, piece_i);

	char pieces[5];
	pieces[0] = 'K';
	pieces[1] = 'Q';
	pieces[2] = 'N';
	pieces[3] = 'B';
	pieces[4] = 'P';

	for (int k = start + 1; k < 5; k++) { //for each piece
		for (int i = start + 1; i < end; i++){ //in the space between the pawn and the rook
			int tile = get_bit_from_pos(i, pawn_j);
			if (board.getBit(pieces[k], color, tile)) {
				return false;
			}
		}
	}
	return true;
}

int nb_protecting_rooks(CFBoard &board, bool &color, int& pawn_i, int& pawn_j) {
	int count = 0;
	for (int i = 0; i< 8: i++){ //for each position in the row
		int tile = get_bit_from_pos(i, pawn_j);
		if (board.getBit('R', color, tile)) {
			if (is_row_free(board, color, pawn_i, pawn_j, i, pawn_j))) {
				count++;
			}
		}
	}
	return count;
}

int nb_protectcting_kings(CFBoard& board, bool& color, int& pawn_i, int& pawn_j) {
	int count = 0;
	for (int k = -1; k < 2; k++) {
		for (int l = -1; l < 2; l++) {
			if (is_valid(pawn_i + k, pawn_j + l)) {
				int tile = get_bit_from_pos(pawn_i + k, pawn_j + l);
				if (board.getBit('K', color, tile)) {
					count++;
				}
			}

		}
	}
}

int nb_protecting_queens(CFBoard& board, bool& color, int& pawn_i, int& pawn_j) {

}