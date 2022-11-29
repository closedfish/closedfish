#include <iostream>
#include <stdint.h>
#include <string>

#include <bitset>
#include "CFBoard.h"



CFBoard::CFBoard() { //This is just the starter board.
    pawnBoard = (((1ll << 8) - 1) << 48) + (((1ll << 8) - 1) << 8);
    kingBoard = (1ll << 4) + (1ll << 60);
    queenBoard = (1ll << 3) + (1ll << 59);
    rookBoard = (1ll << 0) + (1ll << 7) + (1ll << 56) + (1ll << 63);
    knightBoard = (1ll << 1) + (1ll << 6) + (1ll << 57) + (1ll << 62);
    bishopBoard = (1ll << 2) + (1ll << 5) + (1ll << 58) + (1ll << 61);
    blackBoard = (1ll << 16) - 1;
    whiteBoard = ((1ll << 16) - 1) << 48;

	enPassantTarget = -1;
	castleCheck = (1 << 4) - 1;

    turn = 0;
}

void CFBoard::fromFEN(std::string FEN){
    // configure the current board from input FEN
}
std::string toFEN(){
	std::string result = "";


	return result;
}

void CFBoard::addPiece(char pieceType, bool color, int tile) {
    uint64_t PieceBoard = 1ll << tile;
    removePiece(tile);

    switch (pieceType) {
    case 'P':
        pawnBoard = pawnBoard | PieceBoard;
        break;
    case 'K':
        kingBoard = kingBoard | PieceBoard;
        break;
    case 'Q':
        queenBoard = queenBoard | PieceBoard;
        break;
    case 'R':
        rookBoard = rookBoard | PieceBoard;
        break;
    case 'N':
        knightBoard = knightBoard | PieceBoard;
        break;
    case 'B':
        bishopBoard = bishopBoard | PieceBoard;
        break;
    }

    if (color) {
        blackBoard = blackBoard | PieceBoard;
    }
    else {
        whiteBoard = whiteBoard | PieceBoard;
    }
}
void CFBoard::removePiece(int tile) {
    uint64_t AntiPieceBoard = ~(1ll << tile);
    pawnBoard = pawnBoard & AntiPieceBoard;
    kingBoard = kingBoard & AntiPieceBoard;
    queenBoard = queenBoard & AntiPieceBoard;
    rookBoard = rookBoard & AntiPieceBoard;
    knightBoard = knightBoard & AntiPieceBoard;
    bishopBoard = bishopBoard & AntiPieceBoard;
    blackBoard = blackBoard & AntiPieceBoard;
    whiteBoard = whiteBoard & AntiPieceBoard;
    blackBoard = blackBoard & AntiPieceBoard;
    whiteBoard = whiteBoard & AntiPieceBoard;
}

//void CFBoard::naiveMovePiece(int starttile, int endtile) {}

bool CFBoard::getBit(char pieceType, bool color, int tile) {
    uint64_t Board = getPieceBitBoard(pieceType) & getColorBitBoard(color);
    return (Board << tile);
}

uint64_t CFBoard::getPieceBitBoard(char pieceType) {
    switch (pieceType) {
    case 'P':
        return pawnBoard;
    case 'K':
        return kingBoard;
    case 'Q':
        return queenBoard;
    case 'R':
        return rookBoard;
    case 'N':
        return knightBoard;
    case 'B':
        return bishopBoard;
    }
}
uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return blackBoard;
    }
    return whiteBoard;
}

std::string CFBoard::getRepr() {
    std::string repr = "|";
    for (int i = 0; i < 64; i++) {
        repr += " ";
        if ((blackBoard >> i) & 1) {

            if ((pawnBoard >> i) & 1) {
                repr += "p";
            }
            else if ((kingBoard >> i) & 1) {
                repr += "k";
            }
            else if ((queenBoard >> i) & 1) {
                repr += "q";
            }
            else if ((rookBoard >> i) & 1) {
                repr += "r";
            }
            else if ((knightBoard >> i) & 1) {
                repr += "n";
            } else {
                repr += "b";
            }
        }
        else if ((whiteBoard >> i) & 1) {
            if ((pawnBoard >> i) & 1) {
                repr += "P";
            }
            else if ((kingBoard >> i) & 1) {
                repr += "K";
            }
            else if ((queenBoard >> i) & 1) {
                repr += "Q";
            }
            else if ((rookBoard >> i) & 1) {
                repr += "R";
            }
            else if ((knightBoard >> i) & 1) {
                repr += "N";
            } else {
                repr += "B";
            }
        }
        else {
            repr += ".";
        }
        repr += " |";
        if ((i + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}


