#include <iostream>
#include <stdint.h>
#include <cctype>

#include "CFBoard.h"


void CFBoard::fromFEN(std::string FEN) {
    // configure the current board from input FEN
}
std::string CFBoard::toFEN() {
    // convert the current board to FEN
    return "NotYetImplemented";
}

//void CFBoard::naiveMovePiece(int starttile, int endtile) {}

// ========== Methods after this comment are implemented ==========

CFBoard::CFBoard() { //This is just the starter board.
    pawnBoard = (((1ll << 8) - 1) << 48) + (((1ll << 8) - 1) << 8);
    knightBoard = (1ll << 1) + (1ll << 6) + (1ll << 57) + (1ll << 62);
    bishopBoard = (1ll << 2) + (1ll << 5) + (1ll << 58) + (1ll << 61);
    rookBoard = (1ll << 0) + (1ll << 7) + (1ll << 56) + (1ll << 63);
    queenBoard = (1ll << 3) + (1ll << 59);
    kingBoard = (1ll << 4) + (1ll << 60);

    whiteBoard = ((1ll << 16) - 1) << 48;
    blackBoard = (1ll << 16) - 1;

    turn = 0;
}

char CFBoard::pieceIdToChar(int pieceId) {
    pieceId = pieceId >> 1;
    switch (pieceId) {
    case 0:
        return 'P';
    case 1:
        return 'N';
    case 2:
        return 'B';
    case 3:
        return 'R';
    case 4:
        return 'Q';
    case 5:
        return 'K';
    default:
        return '.';
    }
}
bool CFBoard::pieceIdToColor(int pieceId) {
    return pieceId & 1;
}
int CFBoard::pieceCharColorToId(char pieceChar, bool pieceColor) {
    int pieceId = NULL;
    switch (pieceChar) {
    case 'P':
        pieceId = 0;
        break;
    case 'N':
        pieceId = 2;
        break;
    case 'B':
        pieceId = 4;
        break;
    case 'R':
        pieceId = 6;
        break;
    case 'Q':
        pieceId = 8;
        break;
    case 'K':
        pieceId = 10;
        break;
    }
    if (pieceColor) {
        pieceId += 1;
    }
    return pieceId;
}

int CFBoard::getPieceFromCoords(int tile) {
    for (int i = 0; i < 6; i++) {
        if ((getPieceBoardFromIndex(i) >> tile) & 1) {
            return (i << 1) | ((blackBoard >> tile) & 1);
        }
    }
    return -1;
}

uint64_t& CFBoard::getPieceBoardFromIndex(int boardIndex) {
    switch (boardIndex) {
    case 0:
        return pawnBoard;
    case 1:
        return knightBoard;
    case 2:
        return bishopBoard;
    case 3:
        return rookBoard;
    case 4:
        return queenBoard;
    case 5:
        return kingBoard;
    }
}

void CFBoard::addPiece(int pieceId, int tile) {
    uint64_t pieceBoard = 1ll << tile;
    removePiece(tile);

    int pieceType = pieceId >> 1;
    bool color = pieceId & 1;

    uint64_t& targetBoard = getPieceBoardFromIndex(pieceType);
    targetBoard = targetBoard | pieceBoard;

    if (color) {
        blackBoard = blackBoard | pieceBoard;
    }
    else {
        whiteBoard = whiteBoard | pieceBoard;
    }
}
void CFBoard::removePiece(int tile) {
    uint64_t antiPieceBoard = ~(1ll << tile);

    for (int pieceType = 0; pieceType < 6; pieceType++) {
        uint64_t& targetBoard = getPieceBoardFromIndex(pieceType);
        targetBoard = targetBoard & antiPieceBoard;
    }
    blackBoard = blackBoard & antiPieceBoard;
    whiteBoard = whiteBoard & antiPieceBoard;
}

bool CFBoard::getBit(int pieceId, int tile) {
    return (getPieceColorBitBoard(pieceId) >> tile) & 1;
}

uint64_t CFBoard::getColorBitBoard(bool color) {
    if (color) {
        return blackBoard;
    }
    return whiteBoard;
}
uint64_t CFBoard::getPieceColorBitBoard(int pieceId) {
    return getPieceBoardFromIndex(pieceId >> 1) & getColorBitBoard(pieceId & 1);
}


std::string CFBoard::getRepr() {
    std::string repr = "|";
    for (int tile = 0; tile < 64; tile++) {
        repr += " ";

        int pieceId = getPieceFromCoords(tile);
        char pieceChar = pieceIdToChar(pieceId);
        if (pieceId & 1) { pieceChar = tolower(pieceChar); }
        repr += pieceChar;

        repr += " |";
        if ((tile + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}