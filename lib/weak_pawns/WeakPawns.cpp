#include "WeakPawns.h"

/*---DESCRIPTION---

Main functions:

- nbProtectingPieces : Returns the total number of pieces+pawns currenly protecting the piece at the target tile
(can be used to tell if any piece is protected)

- nbProtectingPawns : Returns the total number of pawns currenly protecting the piece at the target tile
(Sub-function of nbProtectingPieces but in closed positions we mainly worl with the pawns and don't need the 
information for the other pieces too)

- isConnected : Tells whether the pawn is connected ( = if it is protected by at least 1 pawn)

- isPassed : Checks if a pawn is passed

- isIsolated : Checks if a pawn is isolated (= if there are no ally pieces/pawns on any of its adjacent tiles)

- protectingTiles :  Gives the tiles that can protect the piece at a certain tile
(if we can get one of the ally pieces on one of the "protecting tiles", then we will protect the target tile)

- blunderBoard : returns the board with both the ally pawns on it and the tiles protected by those pawns
To be used in blunder detection.
We work under the assumption that if the opponents gets on the blunderBoard, he either gets eaten by one
of our pawns, or eats one of our pawns but gets eaten after.

*/

using namespace std; 

inline bool isPositionValid(const int &row, const int &col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

namespace WeakPawns{

	/**
	 * @brief Returns the number of pawns currenly protecting the piece at the target tile.
	 *
	 * @param board : current CFBoard
	 * @param pTile : index of the tile to protect (0...63)
	 * 
	 * @return : number of protecting pawns 
	 */
	int nbProtectingPawns(CFBoard &board, int &pTile){

		bool color = board.getPieceFromCoords(pTile)%2;
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
	 * @param pTile : index of the tile to protect (0...63)
	 * @param boardId : equal to 0/1/2/3/4/5 for P/N/B/R/Q/K (piece_id >> 1)
	 * 
	 * @return : number of pieces of boardId protecting the target tile
	 */
	int nbProtectingPiecesById(CFBoard &board, int &pTile, int boardId){

		int pType  = board.getPieceFromCoords(pTile);
		bool color = pType%2;
		
		if(boardId == 0){ //if pawns, special case (since pawns move differently when they capture)
			return nbProtectingPawns(board, pTile);
		}

		int count = 0;
		board.forceRemovePiece(pTile); //temporarly remove the piece to protect so the other pieces can get on the tile
		uint64_t allyPieceBoard = board.getPieceBoardFromIndex(boardId) & board.getColorBitBoard(color);

		for (int tile = 0; tile <= 63; tile++){
			if((allyPieceBoard & (1ll << tile))!= 0){ //we found an ally piece
				uint64_t moves = board.getLegalMoves((boardId<<1) + color, tile); //get the moves that the ally piece can do
				if(((moves)&(1ll<<pTile))!=0){//if the ally piece can do a move that reaches the tile to protect
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
	 * @param tile : index of the tile to protect
	 * 
	 * @return : number of protecting pieces/pawns 
	 */
	int nbProtectingPieces(CFBoard board, int tile){

		bool color = board.getPieceFromCoords(tile)%2;

		int count = 0;

		for(int i = 0; i <= 5; i++){
			count += nbProtectingPiecesById(board, tile, i);
		}
		return count;
		
	}

	/**
	 * @brief Tells whether the pawn at tile is connected or not
	 * (if it is protected by at least 1 pawn or is protecting at least 1 pawn)
	 *
	 * @param board : current CFBoard
	 * @param tile : index of the tile of the pawn (0...63)
	 * 
	 * @return : true or false
	 */
	bool isConnected(CFBoard &board, int &tile){
		
		int color = board.getPieceFromCoords(tile)%2;
		
		int pi = tile/8;
		int pj = tile%8;

		if(board.getPieceFromCoords((pi+1)*8+(pj+1)) == color){
			return true;
		}
		if(board.getPieceFromCoords((pi-1)*8+(pj+1)) == color){
			return true;
		}
		if(board.getPieceFromCoords((pi+1)*8+(pj-1)) == color){
			return true;
		}
		if(board.getPieceFromCoords((pi-1)*8+(pj-1)) == color){
			return true;
		}

		return false;
		
	}

	/**
	 * @brief Checks if a pawn is passed or not 
	 * (if the colomn between the pawn and the opponent's side of the board if free)
	 *
	 * @param board : current CFBboard
	 * @param tile : index of tile of the pawn (0..63)
	 * 
	 * @return : true or false 
	 */
	bool isPassed(CFBoard &board, int &tile){

		bool color = board.getPieceFromCoords(tile)%2;
		uint64_t straight_line = board.getLegalMoves(6 + color, tile); // we imagine that the pawn in a rook


		int to_reach;
		if(color){//if black, we go to the last row
			to_reach = 7*8 + tile%8;
		}
		else{///if white, we go to the top row
			to_reach = tile%8;
		}

		if(straight_line & (1ll<<to_reach)){
			return true;
		}
		return false;
	}

	/**
	 * @brief Checks if a pawn is isolated (if there are no ally pawns on col+1 and col-1)
	 * @param board : current CFboard
	 * @param tile : index of tile of the pawn (0...63)
	 * 
	 * @return : true or false
	 */
	bool isIsolated(CFBoard &board, int &tile){ 
		
		int color = board.getPieceFromCoords(tile)%2;
		int pi = tile/8;
		int pj = tile%8;
		for(int i = 0; i <= 7; i++){
			if(board.getPieceFromCoords(i*8 + pj + 1) == color){
				return false;
			}
			if(board.getPieceFromCoords(i*8 + pj - 1) == color){
				return false;
			}

		}
		return true;	
	}

	/**
	 * @brief Returns a bit board with 1s on the tiles that can protect the target tile
	 * by putting pawn on the "protecting tile"
	 * (helper function for protectingTilesById)
	 *
	 * @param board : <CFBoard> current board
	 * @param ptile : <int> index of the tile we want to protect
	 * 
	 * @return : bitboard
	 */
	uint64_t protectingTilesForPawns(CFBoard &board, int &pTile){
		
		bool color = board.getPieceFromCoords(pTile)%2;
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
	 * @param pTile : index of the tile we want to protect (0...63)
	 * @param boardId : equal to 0/1/2/3/4/5 for P/N/B/R/Q/K (piece_id >> 1)
	 * 
	 * @return: bitboard
	 */
	uint64_t protectingTilesForId(CFBoard &board, int pTile, int boardId){
		
		int color = board.getPieceFromCoords(pTile)%2;
		uint64_t result = 0;

		if(boardId == 0){ //if pawn
			return protectingTilesForPawns(board, pTile);
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
	 * @param tile : index of the tile we want to protect (0...63)
	 * 
	 * @return: bitboard
	 */
	uint64_t protectingTiles(CFBoard &board, int tile){
		
		bool color = board.getPieceFromCoords(tile)%2;
		uint64_t result = 0; //output bitboard

		for(int i = 0; i<=5; i++){
			result = result | protectingTilesForId(board,tile, i);
		}

		return result;	
	}

	/**
	 * @brief For debug purposes: returns the string representation of the current board
	 * with a '#' on the free tiles that can protect the target tile
	 *
	 * @param board : current CFBboard
	 * @param tile : index of the tile we want to protect (0...63)
	 * 
	 * @return: string
	 */
	string ReprProtectingTiles(CFBoard board, int tile){
		
		bool color = board.getPieceFromCoords(tile)%2;
		uint64_t proTiles = protectingTiles(board, tile);
		std::string repr = "|";
		bool isProtectingTile; //0 or 1
		for (int tileI = 0; tileI <= 63; tileI++) {
			isProtectingTile = (proTiles>>tileI)&1;
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

	/**
	 * @brief Returns the bitboard of the tiles protected by pawns 
	 *
	 * @param board : current CFBboard
	 * @param color : white : 0, black : 1
	 * 
	 * @return: bitboard (uint64_t)
	 */
	uint64_t getBoardProtectedByPawns(CFBoard board, bool color){

		uint64_t pawnBoard = board.getPieceBoardFromIndex(0) & board.getPieceColorBitBoard(color);
		uint64_t res = 0ll;

		for(int t = 0; t<=63; t++){
			if((1ll<<t)&pawnBoard){ //found an ally pawn
				
				//get the cartesian coords of the pawn
				int pi = t/8;
				int pj = t%8;
				
				
				if(color == 0){ //if white, we check if the two upper corners positions
					if(isPositionValid(pi - 1 , pj - 1)){ //upper left
						int tile = (pi - 1)*8 + (pj - 1);
						// std::cerr << "upper left white " << tile << '\n';
						res |= (1ll<<tile);
					}
					if(isPositionValid(pi - 1 , pj + 1)){ //upper right
						int tile = (pi - 1)*8 + (pj + 1);
						// std::cerr << "upper right white " << tile << '\n';
						res |= (1ll<<tile);
					}
				}
				else{ //if black, we check the two bottom corners positions
					if(isPositionValid(pi + 1 , pj - 1)){
						int tile = (pi + 1)*8 + (pj - 1);
						res |= (1ll<<tile);
					}
					if(isPositionValid(pi + 1 , pj + 1)){
						int tile = (pi + 1)*8 + (pj + 1);
						res |= (1ll<<tile);
					}
				}
					
			}
		}

		return res;
	}

	/**
	 * @brief For blunder detectiong : returns the board with the ally pawns + the tiles protected by the pawns
	 * If the opponent crosses that board, they get eaten
	 * 
	 * !!! to be computed BEFORE the opponent's move !!!
	 *
	 * @param board : current CFBboard
	 * @param color : ally color 0 or 1
	 * 
	 * @return: string
	 */
	uint64_t blunderBoard(CFBoard board, bool color){
		return getBoardProtectedByPawns(board, color) & (board.getPieceBoardFromIndex(0) & board.getColorBitBoard(color));
	}

	/**
	 * @brief For debug purposes: returns the string representation of the board protected by pawns
	 *
	 * @param board : current CFBboard
	 * @param color : ally color
	 * 
	 * @return: string
	 */
	string ReprProtectedByPawn(CFBoard board, bool color){
		
		uint64_t protByPawns = getBoardProtectedByPawns(board, color);
		std::string repr = "|";
		bool isProtectedTile; //0 or 1
		for (int tileI = 0; tileI <= 63; tileI++) {
			isProtectedTile = (protByPawns>>tileI)&1;
			repr += " ";

			int pieceIdI = board.getPieceFromCoords(tileI);
			char pieceCharI = board.pieceIdToChar(pieceIdI);
			if(pieceCharI != '.'){
				repr += pieceCharI;
			}
			else{
				repr+= isProtectedTile?"X":".";
			}

			repr += " ";
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
