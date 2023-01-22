#include "PieceMovements.h"

using namespace std;
using namespace WeakPawns;


/**
 * @brief : Returns a bitboard with 1s on the tiles that the opponent can eat
 * 
 * @param board : current CFBoard
 * @param color : ally color (white : 0, black : 1)
 * 
 * @return : bitboard
*/
uint64_t getDangerousTiles(CFBoard board, bool color){

    uint64_t dangerBoard = 0ll;
    bool opp_color = (color+1)%2; //get the opponent's color

    for(int t = 0; t<=63; t++){ //we check each tile
        int piece = board.getPieceFromCoords(t);
        if((piece != -1) && (piece%2 == opp_color)){ //we found an opponent piece
            dangerBoard = dangerBoard | board.getLegalMoves(piece, t);
        }
    }
    return dangerBoard | getBoardProtectedByPawns(board, opp_color);
}


/**
 * @brief : Tells if a tile is dangerous (if one of the opponent's pieces can eat an ally piece at tile)
 * 
 * @param board : current CFBoard
 * @param bool: ally color
 * @param tile : index of the tile (0...63)
 * 
 * @return : true or false
*/
bool isTileDangerous(CFBoard board, bool color, int tile){

    if((1ll<<tile)&getDangerousTiles(board, color)){
        return true;
    }
    return false;
}

/**
 * @brief : Tells if a piece can move out of its tile
 * 
 * @param board : current CFBoard
 * @param tile : index of the tile (0...63) where the piece is currently on
 * 
 * @return : 0 if it can't move, 1 if it can move but only to a dangerous tile, 2 if it can move on a safe tile
*/
int canPieceMoveOut(CFBoard &board, int tile){

    int pieceId = board.getPieceFromCoords(tile);
    bool color = pieceId%2;
    uint64_t moves = board.getLegalMoves(pieceId, tile);
    if(moves == 0){ //it can't move
        return 0;
    }

    for(int t = 0; t<=63; t++){
        if(moves&(1ll << t)){
            if(isTileDangerous(board, color, tile) == false){
                return 2;
            }
        }
    }
    return 1;
}

/**
 * @brief : Gives the possible movements of a piece 
 * 
 * @param board : current CFBoard
 * @param tile : index of the tile (0...63) where the piece is currently on
 * 
 * @return : an array of size 3 where:
 * - the first element is a bitboard with all the moves the piece can directly do
 * - the second element is a bitboard with all the positions that are currently taken by an ally piece but 
 * are accessible by our piece and the pieces occupying these positions can move out in 1 move 
 * - the third element is a bitboard with all the positions that are currently taken by an ally piece but the 
 * pieces occupying these positions can move out in 1 move BUT they can only go on a square that is dangerous
 * (the positions described by the third element are a "subset" of the postions in the second element)
 * 
*/
uint64_t* getPieceMovements(CFBoard &board, int tile){

    int pieceId = board.getPieceFromCoords(tile);
    bool color = pieceId%2;

    uint64_t* result = new uint64_t[3];
    uint64_t directPieceMovements = board.getLegalMoves(pieceId, tile);
    uint64_t pieceMovements = 0ll;
    uint64_t losses = 0ll;

    for(int t = 0; t<= 63; t++){
        int piece = board.getPieceFromCoords(t);
        if((piece%2 != -1) && (piece%2 == color)){ //we found an ally piece
            board.forceRemovePiece(tile); //we remove our piece 
            uint64_t prT = protectingTilesForId(board, t, pieceId >> 1);
            if(prT&(1ll<<tile)){ //if the tile of our piece is protecting the tile t
                int moveOut = canPieceMoveOut(board, t);
                if(moveOut > 0){ //the piece at tile t can move out 
                    pieceMovements += (1ll<<t); 
                }
                if(moveOut == 1){ //the piece at tile t can move out but it can only go to a dangerous tile
                    losses += (1ll<<t);
                }
            }
            board.forceAddPiece(pieceId, tile); //we add the piece back so the board remains unchanged
        }
    }

    result[0] =  directPieceMovements;
    result[1] = pieceMovements;
    result[2] = losses;

    return result;
}

/**
 * @brief : Gives you the opponent's last move from the previous and current board 
 * 
 * @param lastBoard : board stored after our turn, before opponent's turn (important!)
 * @param currentBoard : current board after opponent plays
 * @param colorPlayed : color of the opponent
 * 
 * @return 0 if we detenct there have been multiple moves in between, otherwise:
 * an int x = 64*(starttile) + endtile (start and end encoded in one int).
 * 
*/
int getLastMove(CFBoard lastBoard, CFBoard currentBoard, bool colorPlayed){

    uint64_t lastColorBoard = lastBoard.getColorBitBoard(colorPlayed);
    uint64_t currentColorBoard = currentBoard.getColorBitBoard(colorPlayed);

    uint64_t diffBoard = lastColorBoard ^ currentColorBoard;

    if (__builtin_popcountll(diffBoard) > 2){return -1;}

    return 64*(__builtin_ctzll(diffBoard & lastColorBoard)+1) + __builtin_ctzll(diffBoard & currentColorBoard)+1;
}