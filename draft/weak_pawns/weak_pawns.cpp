// Chess_AI.cpp : définit le point d'entrée de l'application.
//

#include "../../lib/board_implementation/CFBoard.cpp"
#include "../../lib/board_implementation/naiveCheckCheck.cpp"
#include "../../lib/board_implementation/CFBoard.h"

#include <iostream>
using namespace std;

int get_bit_from_pos(int i, int j) {
	/**
     * @brief Converts a position in cartesians coordinates to a tile in the CFBoard
     *
     * @param i : <int> row of the position
	 * @param j : <int> colomn of the position
	 * 
     * @return : <int> tile in the CFBoard
     */

	return i*8 + j;
}

bool is_valid(int i, int j) {
	/**
     * @brief Checks if a position fits in the board 
     *
     * @param i : <int> row of the position
	 * @param j : <int> colomn of the position
	 * 
     * @return : <bool> whether the position fits or not on the board
     */

	if (i >= 0 && i <= 7 && j >= 0 && j <= 7) {
		return true;
	}
	return false;
}

int nb_protecting_pawns(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
	/**
     * @brief Returns the number of pawns currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of pawns protecting the target pawn
     */

	int count = 0;
	if(color){ //if black, we check if the two upper corners positions
		if (pawn_i >= 1) {
			if (pawn_j >= 1) {
				int tile = get_bit_from_pos(pawn_i - 1 , pawn_j - 1);
				if (board.getBit(color,tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = get_bit_from_pos(pawn_i - 1, pawn_j + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}
	}else{ //if white, we check the two bottom corners positions
		if(pawn_i <= 6){
			if (pawn_j >= 1) {
				int tile = get_bit_from_pos(pawn_i + 1, pawn_j - 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = get_bit_from_pos(pawn_i + 1, pawn_j + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}
	}

	return count;
}

int nb_protecting_horses(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Returns the number of knights currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of horses protecting the target pawn
     */

	int count = 0;
	int nId;

	if(color){ //for blacks
		nId = 3;
	}else{ //for whites
		nId = 2;
	}

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (i != j && i != 0 && j != 0 && abs(j) != abs(i)){ //check if the position is valid for a horse
				if (is_valid(pawn_i + i, pawn_j + j)){ //check if the tile is in the board
					int tile = get_bit_from_pos(pawn_i + i, pawn_j + j);
					//std::cout<<board.getBit(3, tile)<<std::endl;
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
	/**
     * @brief Checks if the row is free between two pieces of the same row
     *
     * @param board : <CFBoard> current board
	 * @param pawn_i : <int> row where the two pieces are
	 * @param start : <int> leftmost piece index
	 * @param end : <int> rightmost piece index
	 * 
     * @return : <bool> returns whether or not the row is free
     */

	for (int j = start + 1; j <= end-1; j++) { //for each position between start and end
			int tile = get_bit_from_pos(pawn_i, j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

bool is_colomn_free(CFBoard &board, int &pawn_j, int& start, int& end){
	/**
     * @brief Checks if the colomn is free between two pieces of the same colomn
     *
     * @param board : <CFBoard> current board
	 * @param pawn_j : <int> colomn where the two pieces are
	 * @param start : <int> upper piece index
	 * @param end : <int> bottom piece index
	 * 
     * @return : <bool> returns whether or not the colomn is free
     */

	for (int i = start + 1; i <= end-1; i++) { //for each position between start and end
			int tile = get_bit_from_pos(i, pawn_j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

int nb_protecting_rooks(CFBoard &board, bool &color, int& pawn_i, int& pawn_j){
	/**
     * @brief Returns the number of rooks currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of rooks protecting the target pawn
     */

	int count = 0;
	int tile;

	int rId;

	if(color){ //if black
		rId = 7;
	}else{//if white
		rId = 6;
	}

	//we check if there is a rook on the same row
	for (int j = 0; j<= 7; j++){ //for each position
		tile = get_bit_from_pos(pawn_i, j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same row
			int start = std::min(pawn_j, j); //we set the boundaries of the segments we are gonna search on
			int stop = std::max(pawn_j, j);
			if (is_row_free(board, pawn_i, start, stop)){ //if the row is free between the pawn and the rook
				count++;
			}
		}

		//we check if there is a rook on the same colomn
		int i = j;
		tile = get_bit_from_pos(i, pawn_j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same colomn
			int start = std::min(pawn_i, i);
			int end = std::max(pawn_i, i);
			if (is_colomn_free(board, pawn_j, start, end)){ //if the colomn is free between the pawn and the rook
				count++;
			}
		}
	}

	return count;
}

int nb_protecting_kings(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
	/**
     * @brief Returns the number of kings currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of kings protecting the target pawn (0 or 1)
     */

	
	int kId;

	if(color){ //if black
		kId = 11;
	}else{//if white
		kId = 10;
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
	/**
     * @brief Checks if the diagonal between a pawn and a piece is free
     *
     * @param board : <CFBoard> current board
     * @param pawn_i : <int> row of the pawn
	 * @param piece_i : <int> row of the piece
	 * @param pawn_j : <int> colomn of the pawn
	 * @param piece_j : <int> colomn of the piece
	 * 
     * @return : <bool> whether the diagonal is free or not
     */

	int ud;
	int lr;

	//we start checking from the position of the pawn

	if(pawn_i < piece_i){
		ud = 1; //if the piece is below the pawn, we need to go down
	}else{
		ud = -1; //else we go up
	}
	if(pawn_j < piece_j){
		lr = 1; //if the piece is to the left of the pawn, we go left
	}else{
		lr = -1; //else we go right
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
	/**
     * @brief Returns the number of bishops currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of bishops protecting the target pawn
     */

	int count = 0;
	int bId;

	if(color){
		bId = 5;
	}else{
		bId = 4;
	}

	for(int i = 0; i < 8; i++){
		for(int k = -1; k <= 1; k++){
			for(int j = -1; j <= 1; j++){
				if(j != 0 && k != 0){ 
					int bish_i = pawn_i + i*k;
					int bish_j = pawn_j + i*j;
					if(is_valid(bish_i, bish_j)){ //if the position is valid
						int tile = get_bit_from_pos(bish_i, bish_j); 
						if(board.getBit(bId, tile)){ //if there is a bishop on this position
							if(is_diag_free(board, pawn_i, bish_i, pawn_j, bish_j)){ //if the diag is free
								count++;
							}
						} 
					}
				}
			}
		}
	}

	return count;


}

int nb_protecting_queens(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Returns the number of queens currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> number of queens protecting the target pawn
     */


	int count = 0;
	int qId;
	int tile;

	if(color){
		qId = 9;
	}else{
		qId = 10;
	}

	for(int i = 0; i < 8; i++){
		for(int k = -1; k <= 1; k++){ //we check if they are on the same diag
			for(int j = -1; j <= 1; j++){
				if(j != 0 && k != 0){ 
					int queen_i = pawn_i + i*k;
					int queen_j = pawn_j + i*j;
					if(is_valid(queen_i, queen_j)){ //if the position is valid
						tile = get_bit_from_pos(queen_i, queen_j); 
						if(board.getBit(qId, tile)){ //if there is a bishop on this position
							if(is_diag_free(board, pawn_i, queen_i, pawn_j, queen_j)){ //if the diag is free
								count++;
							}
						} 
					}
				}
			}
		}

		//we check if they are on the same colomn
		tile = get_bit_from_pos(i, pawn_j);
		if (board.getBit(qId, tile)){ //if we found a rook on the same colomn
			int start = std::min(pawn_i, i);
			int end = std::max(pawn_i, i);
			if (is_colomn_free(board, pawn_j, start, end)){ //if the colomn is free between the pawn and the rook
				count++;
			}
		}

		//we check if there is a rook on the same row
		tile = get_bit_from_pos(pawn_i, i);
		if (board.getBit(qId, tile)){ //if we found a rook on the same row
			int start = std::min(pawn_j, i); //we set the boundaries of the segments we are gonna search on
			int stop = std::max(pawn_j, i);
			if (is_row_free(board, pawn_i, start, stop)){ //if the row is free between the pawn and the rook
				count++;
			}
		}
	}

	return count;

}

int nBProtectingPieces(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Returns the total number of pieces/pawns currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> total number of pieces/pawns protecting the target pawn
     */

	int count = 0;

	count += nb_protecting_pawns(board, color, pawn_i, pawn_j);
	count += nb_protecting_horses(board, color, pawn_i, pawn_j);
	count += nb_protecting_bishops(board, color, pawn_i, pawn_j);
	count += nb_protecting_kings(board, color, pawn_i, pawn_j);
	count += nb_protecting_queens(board, color, pawn_i, pawn_j);
	count += nb_protecting_rooks(board, color, pawn_i, pawn_j);

	return count;
	
}

bool isPassed(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Checks if a pawn is passed or not
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <bool> whether the pawn is passed or not
     */

	if (color){//if black, the pawn is passed once we reached the row i = 7 (last row)
		if(is_colomn_free(board, pawn_j, pawn_i, 7)){
			return true;
		}
	}else{ //if white, the pawn is passed once we reached row i = 0 (first row)
		if(is_colomn_free(board, pawn_j, 0, pawn_i)){
			return true;
		}
	}
}

