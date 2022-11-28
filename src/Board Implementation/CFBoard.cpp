#include <iostream>
#include <stdint.h>

#include "CFBoard.h""



CFBoard::CFBoard() { //This is just the starter board.
    PawnBoard = (((one << 8) - 1) << 48) + (((one << 8) - 1) << 8);
    KingBoard = (one << 4) + (one << 60);
    QueenBoard = (one << 3) + (one << 59);
    RookBoard = (one << 0) + (one << 7) + (one << 56) + (one << 63);
    KnightBoard = (one << 1) + (one << 6) + (one << 57) + (one << 62);
    BishopBoard = (one << 2) + (one << 5) + (one << 58) + (one << 61);
    BlackBoard = (one << 16) - 1;
    WhiteBoard = ((one << 16) - 1) << 48;

    turn = 1;
}

void CFBoard::fromFEN(std::string FEN){
    // configure the current board from input FEN
}
std::string toFEN(){
    // convert the current board to FEN
    return "NotYetImplemented";
}

void CFBoard::addPiece(char pieceType, bool color, int tile) {
    uint64_t PieceBoard = one << tile;
    removePiece(tile);

    switch (pieceType) {
    case 'P':
        PawnBoard = PawnBoard | PieceBoard;
        break;
    case 'K':
        KingBoard = KingBoard | PieceBoard;
        break;
    case 'Q':
        QueenBoard = QueenBoard | PieceBoard;
        break;
    case 'R':
        RookBoard = RookBoard | PieceBoard;
        break;
    case 'N':
        KnightBoard = KnightBoard | PieceBoard;
        break;
    case 'B':
        BishopBoard = BishopBoard | PieceBoard;
        break;
    }

    if (color) {
        BlackBoard = BlackBoard | PieceBoard;
    }
    else {
        WhiteBoard = WhiteBoard | PieceBoard;
    }
}
void CFBoard::removePiece(int tile) {
    uint64_t AntiPieceBoard = ~(one << tile);
    PawnBoard = PawnBoard & AntiPieceBoard;
    KingBoard = KingBoard & AntiPieceBoard;
    QueenBoard = QueenBoard & AntiPieceBoard;
    RookBoard = RookBoard & AntiPieceBoard;
    KnightBoard = KnightBoard & AntiPieceBoard;
    BishopBoard = BishopBoard & AntiPieceBoard;
    BlackBoard = BlackBoard & AntiPieceBoard;
    WhiteBoard = WhiteBoard & AntiPieceBoard;
    BlackBoard = BlackBoard & AntiPieceBoard;
    WhiteBoard = WhiteBoard & AntiPieceBoard;
}

//void CFBoard::naiveMovePiece(int starttile, int endtile) {}

bool CFBoard::getBit(char pieceType, bool color, int tile) {
    uint64_t Board = getPieceBitBoard(pieceType) & getColorBitBoard(color);
    return (Board << tile);
}

uint64_t CFBoard::getPieceBitBoard(char pieceType) {
    switch (pieceType) {
    case 'P':
        return PawnBoard;
    case 'K':
        return KingBoard;
    case 'Q':
        return QueenBoard;
    case 'R':
        return RookBoard;
    case 'N':
        return KnightBoard;
    case 'B':
        return BishopBoard;
    }
}
uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return BlackBoard;
    }
    return WhiteBoard;
}

std::string CFBoard::getRepr() {
    std::string repr = "|";
    for (int i = 0; i < 64; i++) {
        repr += " ";
        if ((BlackBoard >> i) & 1) {

            if ((PawnBoard >> i) & 1) {
                repr += "p";
            }
            else if ((KingBoard >> i) & 1) {
                repr += "k";
            }
            else if ((QueenBoard >> i) & 1) {
                repr += "q";
            }
            else if ((RookBoard >> i) & 1) {
                repr += "r";
            }
            else if ((KnightBoard >> i) & 1) {
                repr += "n";
            } else {
                repr += "b";
            }
        }
        else if ((WhiteBoard >> i) & 1) {
            if ((PawnBoard >> i) & 1) {
                repr += "P";
            }
            else if ((KingBoard >> i) & 1) {
                repr += "K";
            }
            else if ((QueenBoard >> i) & 1) {
                repr += "Q";
            }
            else if ((RookBoard >> i) & 1) {
                repr += "R";
            }
            else if ((KnightBoard >> i) & 1) {
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