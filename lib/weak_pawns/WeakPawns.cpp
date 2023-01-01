#include "../board_implementation/CFBoard.cpp"
#include "../board_implementation/naiveCheckCheck.cpp"
#include "../board_implementation/CFBoard.h"

#include <iostream>

/*---DESCRIPTION---

Main functions:

- nbProtectingPieces : Returns the total number of pieces/pawns currenly protecting the piece at the target tile
(can be used to tell if any piece is protected)

- isConnected : Tells whether the pawn at tile is connected or not (if it is protected by at least 1 pawn)

- isPassed : Checks if a pawn is passed or not 

- isIsolated : Checks if a pawn is isolated (if there are no ally pieces/pawns on any of its adjacent tiles)

- prTiles :  Gives the tiles that can protect the piece at a certain tile
(if we can get one of the ally pieces on one of the "protecting tiles", then we will protect the target tile)

*/

using namespace std; 

namespace WeakPawns{

	/**
	 * @brief Returns the number of pawns currenly protecting the piece at the target tile.
	 *
	 * @param board : current CFBoard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param pTile : index of the tile to protect (0...63)
	 * 
	 * @return : number of protecting pawns 
	 */

	int nbProtectingPawns(CFBoard &board, bool &color, int &pTile){

		int pi = pTile/8;
		int pj = pTile%8;
		
		int count = 0;

		if(color){ //if black, we check if the two upper corners positions
			if(isPositionValid(pi - 1 , pj - 1)){
				int tile = (pi - 1)*8 + (pj - 1);
				if (board.getBit(color,tile)){
					count++;
				}
			}
			if(isPositionValid(pi - 1 , pj + 1)){
				int tile = (pi - 1)*8 + (pj + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}
		else{ //if white, we check the two bottom corners positions
			if(isPositionValid(pi + 1 , pj - 1)){
				int tile = (pi + 1)*8 + (pj - 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
			if(isPositionValid(pi + 1 , pj + 1)){
				int tile = (pi + 1)*8 + (pj + 1);
				if (board.getBit(color, tile)){
					count++;
				}
			}
		}

		return count;

	}

	/**
	 * @brief Returns the number of boardId currenly protecting the piece at the target tile
	 *
	 * @param board : current CFBoard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param pTile : index of the tile to protect (0...63)
	 * @param pType : type of the piece to protect equal to 0/2/4/6/8/10 for P/N/B/R/Q/K, +1 if the piece is black.
	 * @param boardId : equal to 0/1/2/3/4/5 for P/N/B/R/Q/K (piece_id >> 1)
	 * 
	 * @return : number of pieces of boardId protecting the target tile
	 */
	int nbProtectingPiecesById(CFBoard &board, bool &color, int &pTile, int pType, int boardId){
		
		if(boardId == 0){ //if pawns, special case (since pawns move differently when they capture)
			return nbProtectingPawns(board, color, pTile);
		}

		int count = 0;
		board.forceRemovePiece(pTile); //temporarly remove the piece to protect so the other pieces can get on the tile
		uint64_t allyPieceBoard = board.getPieceBoardFromIndex(boardId) & board.getColorBitBoard(color);

		for (int tile = 0; tile <= 63; tile++){
			if((allyPieceBoard & (1ll << tile))!= 0){ //we found an ally piece on tile
				uint64_t moves = board.getLegalMoves((boardId<<1) + color, tile); //get the moves that our piece can do
				if(((moves)&(1ll<<pTile))!=0){//if the piece can do a move that reaches the tile to protect
					count++;
				}
			}
		}

		board.forceAddPiece(pType, pTile); //add the piece back

		return count;

	}

	/**
	 * @brief Returns the total number of pieces/pawns currenly protecting the piece at the target tile
	 * (can be used to tell if any piece is protected)
	 *
	 * @param board : current CFBboard
	 * @param color : color of the piece to protect (white : 0, black : 1)
	 * @param tile : index of the tile to protect
	 * @param type : type of the piece to protect
	 * 
	 * @return : number of protecting pieces/pawns 
	 */
	int nbProtectingPieces(CFBoard board, bool color, int tile, int type){

		int count = 0;

		for(int i = 0; i <= 5; i++){
			count += nbProtectingPiecesById(board, color, tile, type, i);
		}
		return count;
		
	}

	/**
	 * @brief Tells whether the pawn at tile is connected or not
	 * (if it is protected by at least 1 pawn)
	 *
	 * @param board : current CFBoard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param tile : index of the tile of the pawn (0...63)
	 * 
	 * @return : true or false
	 */
	bool isConnected(CFBoard &board, bool &color, int &tile){
		if(nbProtectingPiecesById(board, color, tile, 0, color) > 0){
			return true;
		}
		return false;
	}

	/**
	 * @brief Checks if a pawn is passed or not 
	 * (if the colomn between the pawn and the opponent's side of the board if free)
	 *
	 * @param board : current CFBboard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param tile : index of tile of the pawn (0..63)
	 * 
	 * @return : true or false 
	 */
	bool isPassed(CFBoard &board, bool &color, int &tile){

		uint64_t straight_line = board.getLegalMoves(6 + color, tile); // we imagine that the pawn in a rook
		for(int i = 0; i<= 7; i++){
			int opponent_side = i; //if white, top
			if (color){
				opponent_side = 63 - i; //if black, bottom
			}
			if((straight_line)&(1<< opponent_side)){ //if we can reach the opponent side with a straight line
				return true;
			}
		}
		return false;
	}

	/**
	 * @brief Checks if a pawn is isolated (if there are no ally pieces/pawns on any of its adjacent tiles)
	 *
	 * @param board : current CFboard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param tile : index of tile of the pawn (0...63)
	 * 
	 * @return : true or false
	 */
	bool isIsolated(CFBoard &board, bool &color, int &tile){ 
		
		int count = 0; //counts the number of free tiles around 

		//we imagine that we have a king on the tile and see where it can move
		uint64_t free_nei_tiles = board.getLegalMoves(10+color, tile); 
		for (int tile = 0; tile <= 63; tile++){
			if((free_nei_tiles & (1ll << tile))!= 0){ 
				count ++;
			}
		}

		int pi = tile/7; //get index of the row
		int pj = tile%7; //get the index of the column

		if((pi == 0 && pj == 0) || (pi == 7 && pj == 7) || (pi == 0 && pj == 7) || (pi == 7 && pj == 0)){ //if we are on the corners
			if(count == 3){ //only 3 free tiles are required
				return true;
			}
		}
		else{ //if we aren't on a corner
			if(pi == 0 || pj == 0 || pi == 7 || pj == 7){ //if we are on a side
				if(count == 5){ // we need 5 free tiles
					return true;
				}
			}
			else{ //if we are in the middle
				if(count == 8){
					return true;
				}
			}
		}
		return false;		
	}

	/**
	 * @brief Returns a bit board with 1s on the tiles that can protect the target tile
	 * by putting pawn on the "protecting tile"
	 * (helper function for prTilesById)
	 *
	 * @param board : <CFBoard> current board
	 * @param color : <bool> color of the target pawn (white : 0, black : 1)
	 * @param ptile : <int> index of the tile we want to protect
	 * 
	 * @return : bitboard
	 */
	uint64_t prTilesPawns(CFBoard &board, bool &color, int &pTile){
		
		uint64_t result = 0;

		int pi = pTile/8;
		int pj = pTile%8;

		int tile; 

		if(color){ //if black, we check if the two upper corners positions
			if(isPositionValid(pi - 1 , pj - 1)){
				tile = (pi - 1)*8 + (pj - 1);
				if (board.getPieceFromCoords(tile) == -1){ //if the position if free
					result += (1ll << tile);
				}
			}
			if(isPositionValid(pi - 1 , pj + 1)){
				tile = (pi - 1)*8 + (pj + 1);
				if (board.getPieceFromCoords(tile) == -1){ //if the position if free
					result += (1ll << tile);
				}
			}

		}else{ //if white, we check the two bottom corners positions
			if(isPositionValid(pi + 1 , pj - 1)){
				tile = (pi + 1)*8 + (pj - 1);
				if (board.getPieceFromCoords(tile) == -1){ //if the position if free
					result += (1ll << tile);
				}
			}
			if(isPositionValid(pi + 1 , pj + 1)){
				tile = (pi + 1)*8 +  (pj + 1);
				if (board.getPieceFromCoords(tile) == -1){ //if the position if free
					result += (1ll << tile);
				}
			}
		}

		return result;
	}

	/**
	 * @brief Returns a bit board with 1s on the tiles that can protect the target tile
	 * by putting a piece of BoardId on the "protecting tile"
	 * (helper function for prTiles)
	 * 
	 * @param board : current CFBboard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param ptile : index of the tile we want to protect (0...63)
	 * @param boardId : equal to 0/1/2/3/4/5 for P/N/B/R/Q/K (piece_id >> 1)
	 * 
	 * @return: bitboard
	 */
	uint64_t prTilesById(CFBoard &board, bool &color, int &pTile, int boardId){
		
		uint64_t result = 0;

		if(boardId == 0){ //if pawn
			return prTilesPawns(board, color, pTile);
		}

		// we imagine we have a piece_id on the tile
		uint64_t moves = board.getLegalMoves((boardId<<1) + color, pTile); 
		for (int tile = 0; tile <= 63; tile++){
			if((moves & (1ll << tile)) != 0){ 
				result += (1ll << tile);
			}
		}

		return result;
	}
	
	/**
	 * @brief Returns a bit board with 1s on the tiles that can protect the piece at tile
	 * (if we can get one of the ally pieces on one of the "protecting tiles", then we will protect the target tile)
	 *
	 * @param board : current CFBboard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param tile : index of the tile we want to protect (0...63)
	 * 
	 * @return: bitboard
	 */
	uint64_t prTiles(CFBoard &board, bool &color, int &tile){
		
		uint64_t result = 0; //output bitboard

		for(int i = 0; i<=5; i++){
			result = result | prTilesById(board, color, tile, i);
		}

		return result;	
	}

	/**
	 * @brief For debug purposes: returns the string representation of the current board
	 * with a '#' on the free tiles that can protect the target tile
	 *
	 * @param board : current CFBboard
	 * @param color : color of the target pawn (white : 0, black : 1)
	 * @param tile : index of the tile we want to protect (0...63)
	 * 
	 * @return: string
	 */
	string ReprProtectingTiles(CFBoard board, bool color, int tile){
		
		uint64_t protectingTiles = prTiles(board, color, tile);
		std::string repr = "|";
		bool isProtectingTile; //0 or 1
		for (int tileI = 0; tileI <= 63; tileI++) {
			isProtectingTile = (protectingTiles>>tileI)&1;
			repr += (tileI == tile)?"/":" ";

			int pieceIdI = board.getPieceFromCoords(tileI);
			char pieceCharI = board.pieceIdToChar(pieceIdI);
			if(pieceCharI != '.'){
				repr += pieceCharI;
			}
			else{
				repr+= isProtectingTile?"#":".";
			}

			repr += (tileI == tile)?"/":" ";
			repr += "|";
			if ((tileI + 1) % 8 == 0){
				if((tileI == 63)){
					repr += "\n";
				}
				else{
					repr += "\n|";
				}
			}
		}
		return repr;
	}
}
