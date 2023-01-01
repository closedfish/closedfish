#include "../../lib/board_implementation/CFBoard.cpp"
#include "../../lib/board_implementation/naiveCheckCheck.cpp"
#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/weak_pawns/WeakPawns.cpp"

#include <iostream>
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

    uint64_t dangerBoard = 0;
    bool opp_color = (color+1)%2; //get the opponent's color

    for(int t = 0; t<=63; t++){ //we check each tile
        int piece = board.getPieceFromCoords(t);
        if((piece%2 != -1) && (piece%2 == opp_color)){ //we found an opponent piece
            dangerBoard = dangerBoard | (board.getLegalMoves(piece, t) & board.getColorBitBoard(opp_color));
        }
    }

    return dangerBoard;
}

/**
 * @brief : Tells if a tile is dangerous (if one of the opponent's pieces can eat an ally piece at tile)
 * 
 * @param board : current CFBoard
 * @param color : ally color (white : 0, black : 1)
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
 * @param color : ally color (white : 0, black : 1)
 * @param tile : index of the tile (0...63) where the piece is currently on
 * 
 * @return : 0 if it can't move, 1 if it can move but only to a dangerous tile, 2 if it can move on a safe tile
*/
int canPieceMoveOut(CFBoard &board, bool &color, int tile, int pieceId){

    uint64_t moves = board.getLegalMoves(pieceId, tile);
    if(moves == 0){ //it can't move
        return 0
    }

    for(int t = 0; t<=63; t++){
        if(moves&(1ll << t)){
            if(isTileDangerous(board, color, t) == false){
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
 * @param color : ally color (white : 0, black : 1)
 * @param tile : index of the tile (0...63) where the piece is currently on
 * 
 * @return : an array where:
 * - the first element is a bitboard with all the moves the piece can directly do
 * - the second element is a bitboard with all the positions that are currently taken by an ally piece but 
 * are accessible by our piece and the pieces occupying these positions can move out in 1 move 
 * - the third element is a bitboard with all the positions that are currently taken by an ally piece but the 
 * pieces occupying these positions can move out in 1 move BUT they can only go on a square that is dangerous
 * (the positions described by the third element are a "subset" of the postions in the second element)
 * 
*/
uint64_t* getPieceMovements(CFBoard &board, bool &color, int tile){

    uint64_t result[3];
    uint64_t directPieceMovements = board.getLegalMoves(pieceId, tile);
    uint64_t pieceMovements = 0ll;
    uint64_t losses = 0ll;

    int pieceId = board.getPieceFromCoords(tile);

    for(int t = 0; t<= 63; t++){
        int piece = board.getPieceFromCoords(t);
        if((piece%2 != -1) && (piece%2 == color)){ //we found an ally piece
            board.forceRemovePiece(tile); //we remove our piece 
            prT = prTilesById(board, color, t, pieceId >> 1);
            if(prT&(1ll<<tile)){ //if the tile of our piece is protecting the tile t
                int moveOut = canPieceMoveOut(board, color, t, board.getPieceFromCoords(t));
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