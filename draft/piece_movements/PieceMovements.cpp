#include "../../lib/board_implementation/CFBoard.cpp"
#include "../../lib/board_implementation/naiveCheckCheck.cpp"
#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/weak_pawns/WeakPawns.cpp"

#include <iostream>
using namespace std;
using namespace WeakPawns;


bool isTileDangerous(CFBoard board, bool color, int tile){

    //we imagine that there is a pawn of the opposite color on the tile and see if the 
    //imaginary pawn is protected or not

    if(numProtectingPieces(board,(color+1)%2, tile) != 0){ 
        return true;
    }
    return false;
}

int canPawnMoveOut(CFBoard &board, bool &color, int &p_i, int &p_j){
    
    int tile = getBitFromPos(p_i,p_j);//get the tile of the piece

    if(board.getPieceFromCoords(tile) == 0){ //if it is a white pawn
        int movetile = getBitFromPos(p_i - 1, p_j); //moves up
        if(board.getPieceFromCoords(movetile) == -1){//the tile we can move to is free
            if(isTileDangerous(board, color, movetile)){
                if(numProtectingPieces(board, color, movetile) != 0){
                    return 1; //we move to a dangerous tile but we are protected
                }
                else{
                    return 2; //we move to a dangerous tile and we might lose the piece
                }
            }
            else{
                return 1; //the tile we want to move to isn't dangerous
            }
        }
    }

    if(board.getPieceFromCoords(tile) == 1){ //if it is a black pawn
        int movetile = getBitFromPos(p_i + 1, p_j); //moves down
        if(board.getPieceFromCoords(movetile) == -1){//the tile we can move to is free
            if(isTileDangerous(board, color, movetile)){
                if(numProtectingPieces(board, color, movetile) != 0){
                    return 1; //we move to a dangerous tile but we are protected
                }
                else{
                    return 2; //we move to a dangerous tile and we might lose the piece
                }
            }
            else{
                return 1; //the tile we want to move to isn't dangerous
            }
        }
    }

    return 0; //we can't move away
}

int canKnightMoveOut(CFBoard &board, bool &color, int &p_i, int &p_j){

    
}

int canRookMoveOut(CFBoard &board, bool &color, int &p_i, int &p_j){
    
    int movetile;

    //move to the right
    if(isValid(p_i, p_j + 1)){
        movetile = getBitFromPos(p_i, p_j + 1);
        if(board.getPieceFromCoords(movetile) == -1){ 
            return 1;
        }
    }

    if(isValid(p_i, p_j + 1)){
        movetile = getBitFromPos(p_i, p_j + 1);
        if(board.getPieceFromCoords(movetile) == -1){ 
            return 1;
        }
    }



}
