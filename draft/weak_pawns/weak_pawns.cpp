// Chess_AI.cpp : définit le point d'entrée de l'application.
//

#include "Chess_AI.h"
#include "lib/board_implementation/CFBoard.cpp"
#include "lib/board_implementation/CFBoard.h"


using namespace std;


int get_bit_from_pos(int i, int j) {
'''Returns the corresponding bit position from the coordinates of a tile.
i : row
j : colomn '''

	return i*8 + j;
}

bool is_valid(int &i, int &j) {
''' Returns true if the tile is on the board, false otherwise'''

	if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
		return true;
	}
	return false;
}

int nb_protecting_pawns(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
'''Returns the number of pawns protecting the target pawn'''

	int count = 0;
	if(color == 0){ //if white, we check if the two bottom corners positions
		if (pawn_i <= 6) {
			if (pawn_j >= 1) {
				int tile = get_bit_from_pos(pawn_i + 1, pawn_j - 1);
				if (board.getBit(0, tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = get_bit_from_pos(pawn_i + 1, pawn_j + 1);
				if (board.getBit(0, tile)){
					count++;
				}
			}
		}
	}else{ //if black, we check the two upper corners positions
		if(pawn_i >= 1){
			if (pawn_j >= 1) {
				int tile = get_bit_from_pos(pawn_i - 1, pawn_j - 1);
				if (board.getBit(1, tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = get_bit_from_pos(pawn_i - 1, pawn_j + 1);
				if (board.getBit(1, tile)){
					count++;
				}
			}
		}
	}
	

	return count;
}

int nb_protecting_horses(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
'''Returns the number of horses protecting the target pawn'''

	int count = 0;

	if(color){ //for blacks
		int nId = 3;
	}else{ //for whites
		int nId = 2;
	}

	for (int i = -2; i <= 2; i++) {
		for (int j = -2, j <= 3; j++) {
			if (i != j && i != 0 && j != 0 && abs(j) != abs(i)){ //check if the position is valid for a horse
				if (is_valid(pawn_i + i, pawn_j + j)){ //if the tile is in the board
					int tile = get_bit_from_pos(pawn_i + i, pawn_j + j);
					if (board.getBit(nId, tile)){ //if there is a knight of the pawn's color 
						count++;
					}
				}
			}
		}
	}
	return count;
}

bool is_row_free(CFBoard &board, int &pawn_i, int& start, int& end){
'''Checks if a row is empty between the starting colomn position and the end colomn.
Useful of checking whether a queen or rook is protecting.
'''
	for (int j = start + 1; j <= end-1; j++) { //for each position between start and end
			int tile = get_bit_from_pos(pawn_i, j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

bool is_colomn_free(CFBoard &board, int &pawn_j, int& start, int& end){
'''Checks if a colomn is empty between the starting row position and the end row.
Useful of checking whether a queen or rook is protecting.
'''

	for (int i = start + 1; i <= end-1; i++) { //for each position between start and end
			int tile = get_bit_from_pos(i, pawn_j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

int nb_protecting_rooks(CFBoard &board, bool &color, int& pawn_i, int& pawn_j){
'''Returns the number of protecting rooks'''

	int count = 0;

	if(color){ //if black
		int rId = 7;
	}else{//if white
		int rId = 6;
	}

	for (int j = 0; j<= 7; j++){ //we check if there is a rook on the same row
		int tile = get_bit_from_pos(pawn_i, j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same row
			int start = min(pawn_j, j);
			int stop = max(pawn_j, j);
			if (is_row_free(board, pawn_i, start, end)){ //if the row is free between the pawn and the rook
				count++;
			}
		}
	}

	for (int i = 0; i<= 7; i++){ //we check if there is a rook on the same colomn
		int tile = get_bit_from_pos(i, pawn_j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same colomn
			int start = min(pawn_i, i);
			int stop = max(pawn_i, i);
			if (is_colomn_free(board, pawn_j, start, end)){ //if the colomn is free between the pawn and the rook
				count++;
			}
		}
	}
	return count;
}

int nb_protecting_kings(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
'''Returns the number of protecting kings (0 or 1)'''
	
	if(color){ //if black
		int kId = 11;
	}else{//if white
		int kId = 10;
	}

	for (int k = -1; k < 2; k++) {
		for (int l = -1; l < 2; l++){
			if (is_valid(pawn_i + k, pawn_j + l)){
				int tile = get_bit_from_pos(pawn_i + k, pawn_j + l);
				if (board.getBit(kId, tile)) {
					return 1;
				}
			}
			}
	}

	return 0;
}

bool is_diag_free(CFBoard& board, int &pawn_i, int &piece_i, int &pawn_j, int &piece_j){
'''Checks if the diagolonal between the target pawn and the bishop or queen is free.
Useful of protecting bishops and queens'''

	//we start checking from the position of the pawn

	if(pawn_i < piece_i){
		int ud = 1; //if the piece is below the pawn, we need to go down
	}else{
		int ud = -1; //else we go up
	}
	if(pawn_j < piece_j){
		int lr = 1 //if the piece is to the left of the pawn, we go left
	}else{
		int lr = -1 //else we go right
	}

	int i = pawn_i + ud;
	int j = pawn_j + lr;
	while(abs(i-piece_i)> 0 && abs(j-piece_j)> 0){
		int tile = get_bit_from_pos(i, j);
			if (board.getPieceFromCoords(tile) != -1){
				return false;
			}
		i = i + ud;
		j = j + lr;
	}

	return true;

}

int nb_protecting_bishops(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
'''Returns the number of protecting bishops'''

	count++;

	if(color){
		int bId = 5;
	}else{
		int bId = 4;
	}

	for(int i = 0; i < 64; i++){
		int is_set = board.getPieceBoardFromIndex(bId);
		if(is_set){ //if the i-th bit is set
			int bish_i = i/7;
			int bish_j = i%7;
			//now we check if the bishop is on the same diagonal as the pawn
			//we notice that two elements are on the same diagonal if and only if the absolute value of the difference
			//between their i and j coordinates is the same
			if(abs(bish_i - bish_j) == abs(pawn_i - pawn_j)){
				if(is_diag_free(board, pawn_i, bish_i, pawn_j, bish_j)){
					count ++;
				}
			}

		}
	}

	return count;


}

int nb_protecting_queens(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
'''Returns the number of protecting queens (0 or 1)'''
	if(color){
		int qId = 9;
	}else{
		int qId = 10;
	}

	for(int i = 0; i < 64; i++){
		int is_set = board.getPieceBoardFromIndex(qId);
		if(is_set){ //if the i-th bit is set
			int queen_i = i/7;
			int queen_j = i%7;

			//we check if it is on the same diagonal as the pawn
			if(abs(queen_i - queen_j) == abs(pawn_i - pawn_j)){
				if(is_diag_free(board, pawn_i, queen_i, pawn_j, queen_j)){
					return 1;
				}
			}

			//we check if it is on the same row
			if (queen_i == pawn_i){
				int start = min(pawn_j, queen_j);
				int end = max(pawn_j, queen_j);
				if(is_row_free(board, pawn_i, star, end)){
					return 1;
				}
			}

			//we check if it is on the same colomn
			if (queen_j == pawn_j){
				int start = min(pawn_i, queen_i);
				int end = max(pawn_i, queen_i);
				if(is_row_free(board, pawn_j, star, end)){
					return 1;
				}
			}


		}
	}

	return 0;

}

int nb_protecting_pieces(CFBoard &board, bool &colot, int &pawn_i, int &pawn_j){
'''Returns the number of protectong pieces'''
	count = 0;

	count += nb_protecting_pawns(board, color, pawn_i, pawn_j);
	count += nb_protecting_horses(board, color, pawn_i, pawn_j);
	count += nb_protecting_bishops(board, color, pawn_i, pawn_j);
	count += nb_protecting_kings(board, color, pawn_i, pawn_j);
	count += nb_protecting_queens(board, color, pawn_i, pawn_j);
	count += nb_protecting_rooks(board, color, pawn_i, pawn_j);

	return count;
	
}


