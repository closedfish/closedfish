// Chess_AI.cpp : définit le point d'entrée de l'application.
//

#include "../../lib/board_implementation/CFBoard.cpp"
#include "../../lib/board_implementation/naiveCheckCheck.cpp"
#include "../../lib/board_implementation/CFBoard.h"

#include <iostream>
using namespace std;

int getBitFromPos(int i, int j) {
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

bool isValid(int i, int j) {
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

int numProtectingPawns(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
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
				int tile = getBitFromPos(pawn_i - 1 , pawn_j - 1);
				if (board.getBit(color,tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = getBitFromPos(pawn_i - 1, pawn_j + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}
	}else{ //if white, we check the two bottom corners positions
		if(pawn_i <= 6){
			if (pawn_j >= 1) {
				int tile = getBitFromPos(pawn_i + 1, pawn_j - 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
			if (pawn_j <= 6) {
				int tile = getBitFromPos(pawn_i + 1, pawn_j + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}
	}

	return count;
}

int numProtectingKnights(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
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
				if (isValid(pawn_i + i, pawn_j + j)){ //check if the tile is in the board
					int tile = getBitFromPos(pawn_i + i, pawn_j + j);
					if (board.getBit(nId, tile)){ //if there is a knight of the pawn's color 
						count++;
					}
				}
			}
		}
	}
	return count;
}

bool isRowFree(CFBoard &board, int &pawn_i, int start, int end){
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
			int tile = getBitFromPos(pawn_i, j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

bool isColumnFree(CFBoard &board, int &pawn_j, int start, int end){
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
			int tile = getBitFromPos(i, pawn_j);
			if (board.getPieceFromCoords(tile) != -1){ //if there is any piece, it means the row isn't free
				return false;
			}
		}

	return true;
}

int numProtectingRooks(CFBoard &board, bool &color, int& pawn_i, int& pawn_j){
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
		tile = getBitFromPos(pawn_i, j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same row
			int start = std::min(pawn_j, j); //we set the boundaries of the segments we are gonna search on
			int stop = std::max(pawn_j, j);
			if (isRowFree(board, pawn_i, start, stop)){ //if the row is free between the pawn and the rook
				count++;
			}
		}

		//we check if there is a rook on the same colomn
		int i = j;
		tile = getBitFromPos(i, pawn_j);
		if (board.getBit(rId, tile)){ //if we found a rook on the same colomn
			int start = std::min(pawn_i, i);
			int end = std::max(pawn_i, i);
			if (isColumnFree(board, pawn_j, start, end)){ //if the colomn is free between the pawn and the rook
				count++;
			}
		}
	}

	return count;
}

int numProtectingKings(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
	/**
     * @brief Returns the number of kings currenly protecting the target pawn (0 or 1)
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> 
     */

	
	int kId;

	if(color){ //if black
		kId = 11;
	}else{//if white
		kId = 10;
	}

	for (int k = -1; k < 2; k++) {
		for (int l = -1; l < 2; l++){
			if (isValid(pawn_i + k, pawn_j + l)){
				int tile = getBitFromPos(pawn_i + k, pawn_j + l);
				if (board.getBit(kId, tile)) {
					return 1;
				}
			}
			}
	}

	return 0;
}

bool isDiagFree(CFBoard& board, int pawn_i, int piece_i, int pawn_j, int piece_j){
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
		int tile = getBitFromPos(i, j);
			if (board.getPieceFromCoords(tile) != -1){
				return false;
			}
		i = i + ud;
		j = j + lr;
	}

	return true;

}

int numProtectingBishops(CFBoard &board, bool &color, int &pawn_i, int &pawn_j) {
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
					if(isValid(bish_i, bish_j)){ //if the position is valid
						int tile = getBitFromPos(bish_i, bish_j); 
						if(board.getBit(bId, tile)){ //if there is a bishop on this position
							if(isDiagFree(board, pawn_i, bish_i, pawn_j, bish_j)){ //if the diag is free
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

int numProtectingQueens(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Returns the number of queens currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> 
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
					if(isValid(queen_i, queen_j)){ //if the position is valid
						tile = getBitFromPos(queen_i, queen_j); 
						if(board.getBit(qId, tile)){ //if there is a queen on this position
							if(isDiagFree(board, pawn_i, queen_i, pawn_j, queen_j)){ //if the diag is free
								count++;
							}
						} 
					}
				}
			}
		}

		//we check if they are on the same colomn
		tile = getBitFromPos(i, pawn_j);
		if (board.getBit(qId, tile)){ //if we found a rook on the same colomn
			int start = std::min(pawn_i, i);
			int end = std::max(pawn_i, i);
			if (isColumnFree(board, pawn_j, start, end)){ //if the colomn is free between the pawn and the rook
				count++;
			}
		}

		//we check if there is a rook on the same row
		tile = getBitFromPos(pawn_i, i);
		if (board.getBit(qId, tile)){ //if we found a rook on the same row
			int start = std::min(pawn_j, i); //we set the boundaries of the segments we are gonna search on
			int stop = std::max(pawn_j, i);
			if (isRowFree(board, pawn_i, start, stop)){ //if the row is free between the pawn and the rook
				count++;
			}
		}
	}

	return count;

}

int numProtectingPieces(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Returns the total number of pieces/pawns currenly protecting the target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <int> 
     */

	int count = 0;

	count += numProtectingPawns(board, color, pawn_i, pawn_j);
	count += numProtectingKnights(board, color, pawn_i, pawn_j);
	count += numProtectingBishops(board, color, pawn_i, pawn_j);
	count += numProtectingKings(board, color, pawn_i, pawn_j);
	count += numProtectingQueens(board, color, pawn_i, pawn_j);
	count += numProtectingRooks(board, color, pawn_i, pawn_j);

	return count;
	
}

bool isPassed(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Checks if a pawn is passed or not 
	 * (if the colomn between the pawn and the opponent's side of the board if free)
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <bool> 
     */

	if (color){//if black, the pawn is passed once we reached the row i = 7 (last row)
		if(isColumnFree(board, pawn_j, pawn_i, 7)){
			return true;
		}
	}
	else{ //if white, the pawn is passed once we reached row i = 0 (first row) 
		if(isColumnFree(board, pawn_j, 0, pawn_i)){
			return true;
		}
	}

	return false;
}

bool isIsolated(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){
	/**
     * @brief Checks if a pawn is isolated (if there are no pieces/pawns on any of its adjacent tiles)
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
     * @return : <bool>
     */

	for(int j = -1; j <= 1; j++){
		for(int i = -1; i <= 1; i++){
			if(j != 0){
				if(isValid(pawn_i + i, pawn_j + j)){
					int tile = getBitFromPos(pawn_i + i, pawn_j + j);
					if(board.getBit(color, tile)){
						return false;
					}
				}
			}
		}	
	}
	return true;
}

void pTilesKnight(CFBoard &board, bool &color, int &pawn_i, int &pawn_j, std::vector<int> &result){
    
	/**
     * @brief Adds to the result vector the index of the tiles that can protect the target pawn 
	 * if we get a knight on them
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * @param result : <std::vector<int>> vector that stores all of the potentially protecting tiles
	 * 
     */

	for (int i = -2; i <= 2; i++) {
		for (int j = -2; j <= 2; j++) {
			if (i != j && i != 0 && j != 0 && abs(j) != abs(i)){ //check if the position is valid for a horse
				if (isValid(pawn_i + i, pawn_j + j)){ //check if the tile is in the board
					int tile = getBitFromPos(pawn_i + i, pawn_j + j);
					if (board.getPieceFromCoords(tile) != -1){ 
						result.push_back(tile);
					}
				}
			}
		}
	}
}

void pTilesPawn(CFBoard &board, bool &color, int &pawn_i, int &pawn_j, std::vector<int> &result){

	/**
     * @brief Adds to the result vector the index of the tiles that can protect the target pawn 
	 * if we get a pawn on them
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * @param result : <std::vector<int>> vector that stores all of the potentially protecting tiles
	 * 
     */

	if(color){ //if black, we check if the two upper corners positions
		if (pawn_i >= 1) {
			if (pawn_j >= 1) {
				int tile = getBitFromPos(pawn_i - 1 , pawn_j - 1);
				if (board.getPieceFromCoords(tile) != -1){ //if the position if free
					result.push_back(tile);
				}
			}
			if (pawn_j <= 6) {
				int tile = getBitFromPos(pawn_i - 1, pawn_j + 1);
				if (board.getPieceFromCoords(tile) != -1){ 
					result.push_back(tile);
				}
			}
		}

	}else{ //if white, we check the two bottom corners positions
		if(pawn_i <= 6){
			if (pawn_j >= 1) {
				int tile = getBitFromPos(pawn_i + 1, pawn_j - 1);
				if (board.getPieceFromCoords(tile) != -1){
					result.push_back(tile);
				}
			}
			if (pawn_j <= 6) {
				int tile = getBitFromPos(pawn_i + 1, pawn_j + 1);
				if (board.getPieceFromCoords(tile) != -1){
					result.push_back(tile);
				}
			}
		}
	}
}

void pTilesRook(CFBoard &board, bool &color, int &pawn_i, int &pawn_j, std::vector<int> &result){

	/**
     * @brief Adds to the result vector the index of the tiles that can protect the target pawn 
	 * if we get a rook on them
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * @param result : <std::vector<int>> vector that stores all of the potentially protecting tiles
	 * 
     */


	int tile;

	 //we check if there is a rook on the same row
	for (int j = 0; j<= 6; j++){ //for each position on the row
		if(j!= pawn_j){ //we don't want to be on the same position as the pawn
			tile = getBitFromPos(pawn_i, j);
			if(board.getPieceFromCoords(tile) != -1){
				int start = std::min(pawn_j, j); //we set the boundaries of the segments we are going to search on
				int stop = std::max(pawn_j, j);
				if(isRowFree(board, pawn_i, start, stop)){
					result.push_back(tile);
				}
			}
		}


		//we check if there is a rook on the same colomn
		int i = j; //for each position on the column
		if(i!=pawn_i){
			tile = getBitFromPos(i, pawn_j);
			if(board.getPieceFromCoords(tile) != -1){
				int start = std::min(pawn_i, i+1);
				int end = std::max(pawn_i, i+1);
				if(isColumnFree(board, pawn_j, start, end)){ 
					result.push_back(tile);
				}
			}
		}
	}
}

void pTilesBishop(CFBoard &board, bool &color, int &pawn_i, int &pawn_j, std::vector<int> &result){
	
	/**
     * @brief Adds to the result vector the index of the tiles that can protect the target pawn 
	 * if we get a bishop on them
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * @param result : <std::vector<int>> vector that stores all of the potentially protecting tiles
	 * 
     */

	for(int i = 0; i < 7; i++){//for each position in the diagonal
		for(int k = -1; k <= 1; k++){ //get the directions
			for(int j = -1; j <= 1; j++){
				if(j != 0 && k != 0){ //directions must be either 1 or -1
					int bish_i = pawn_i + i*k;
					int bish_j = pawn_j + i*j;
					if(isValid(bish_i, bish_j)){ //if the position is valid
						int tile = getBitFromPos(bish_i, bish_j); 
						if(board.getPieceFromCoords(tile) != -1){ //if the tile is free
							if(isDiagFree(board, pawn_i, bish_i, pawn_j, bish_j)){ //if the diag is free 
								result.push_back(tile);
							}
						}
					}
				}
			}
		}
	}

}

void pTilesKing(CFBoard &board, bool &color, int &pawn_i, int &pawn_j, std::vector<int> &result){
	
	/**
     * @brief Adds to the result vector the index of the tiles that can protect the target pawn 
	 * if we get the king on them
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * @param result : <std::vector<int>> vector that stores all of the potentially protecting tiles
	 * 
     */

	for (int k = -1; k < 2; k++) {
		for (int l = -1; l < 2; l++){
			if (k!=0 && l!= 0){
				if (isValid(pawn_i + k, pawn_j + l)){
					int tile = getBitFromPos(pawn_i + k, pawn_j + l);
					if (board.getPieceFromCoords(tile) == -1){ //the position is free
						result.push_back(tile);
					}
				}

			}
		}
	}
	
}

std::vector<int> pTiles(CFBoard &board, bool &color, int &pawn_i, int &pawn_j){

	/**
     * @brief Returns the vector that stores the indexes of the tiles that can potentially protect a target pawn
     *
     * @param board : <CFBoard> current board
     * @param color : <bool> color of the target pawn (white : 0, black : 1)
     * @param pawn_i : <int> row of the target pawn
	 * @param pawn_j : <int> colomn of the target pawn
	 * 
	 * @return: <std::vector<int>> 
     */
	
	std::vector<int> result;


	pTilesPawn(board, color, pawn_i, pawn_j, result);
	pTilesRook(board, color, pawn_i, pawn_j, result);
	pTilesKnight(board, color, pawn_i, pawn_j, result);
	pTilesKing(board, color, pawn_i, pawn_j, result);
	pTilesBishop(board, color, pawn_i, pawn_j, result);

	return result;
	
}