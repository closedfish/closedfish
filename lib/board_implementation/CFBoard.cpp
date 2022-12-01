#include "CFBoard.h"
#include <bitset>
#include <cctype>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

void CFBoard::fromFEN(std::string FEN) {
    // configure the current board from input FEN
    // Clearing the board
    pawnBoard = 0LL;
    kingBoard = 0LL;
    queenBoard = 0LL;
    rookBoard = 0LL;
    knightBoard = 0LL;
    bishopBoard = 0LL;
    blackBoard = 0LL;
    whiteBoard = 0LL;
    turn = 0;
    castleBools = 0;
    enPassantTarget = 0;

    // split a string python-like
    auto split = [](const std::string &text, char sep) {
        std::vector<std::string> tokens;
        std::size_t start = 0, end = 0;
        while ((end = text.find(sep, start)) != std::string::npos) {
            tokens.push_back(text.substr(start, end - start));
            start = end + 1;
        }
        tokens.push_back(text.substr(start));
        return tokens;
    };

    // Vector of fields of FEN, index 0: board, 1: current turn, 2: castling, 3:
    // e.p., 4: half-move clock, 5: full-move counter
    std::vector<std::string> fields = split(FEN, ' ');

    // Board
    std::string string_board = fields[0];
    std::vector<std::string> rows = split(string_board, '/');

    for (int row_index = 0; row_index < 8; row_index++) {
        int col_index = 0;
        for (auto ch : rows[row_index]) {
            // ch can be digit meaning consecutive empty squares in a row,
            // or characters representing a piece (upper case if white pieces)
            if (isdigit(ch)) {
                col_index += ch - '0';
            } else {
                int pieceId = pieceCharToId(ch);
                this->addPiece(pieceId, row_index * 8 + col_index);
                col_index++;
            }
        }
    }

    // Current turn
    std::string string_current_turn = fields[1];
    turn = (string_current_turn == "b");

    // Castling
    std::string string_castling = fields[2];
    if (string_castling != "-") {
        // dependent on how you want to store castling availability
        // temporarily I use the first four bits of castleBools for K, Q, k, q
        // respectively
        if (string_castling.find('K') != std::string::npos) {
            castleBools |= 1;
        }
        if (string_castling.find('Q') != std::string::npos) {
            castleBools |= 2;
        }
        if (string_castling.find('k') != std::string::npos) {
            castleBools |= 4;
        }
        if (string_castling.find('q') != std::string::npos) {
            castleBools |= 8;
        }
    }

    // En passant
    std::string string_enpassant = fields[3];
    if (string_enpassant != "-") {
        // 16 bits used for squares a6->h6, then a3->h3
        int col = string_enpassant[0] - 'a';
        int row = string_enpassant[1] - '1';
        if (row == 2) { // 3 rank
            enPassantTarget |= (1 << col + 8);
        } else if (row == 5) { // 6 rank
            enPassantTarget |= (1 << col);
        }
    }
}

std::string CFBoard::toFEN() {
    // convert the current board to FEN
    for (int row = 0; row < 8; row++) {
        for (int column = 0; column < 8; column++) {
        }
    }
}

// ---- to test ----

uint64_t CFBoard::getCardinals(int tile, bool color) {
    uint64_t cardinalBoard = 0;
    uint64_t tempBoard = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    // left
    for (int i = 1; i <= column; i++) {
        tempBoard = (1ll << (tile - i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // right
    for (int i = 1; i <= (7 - column); i++) {
        tempBoard = (1ll << (tile + i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // up
    for (int i = 1; i <= row; i++) {
        tempBoard = (1ll << (tile - 8 * i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    // down
    for (int i = 1; i <= (7 - row); i++) {
        tempBoard = (1ll << (tile + 8 * i));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            cardinalBoard = cardinalBoard | tempBoard;
            break;
        }
        cardinalBoard = cardinalBoard | tempBoard;
    }

    return cardinalBoard;
}

uint64_t CFBoard::getDiagonals(int tile, bool color) {
    uint64_t diagonalBoard = 0;
    uint64_t tempBoard = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    // up-left
    for (int i = 1; i <= (column ^ ((row ^ column) & -(row < column)));
         i++) { // y ^ ((x ^ y) & -(x < y)) = min(x,y)
        tempBoard = (1ll << (tile - (9 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // up-right
    for (int i = 1;
         i <= ((7 - column) ^ ((row ^ (7 - column)) & -(row < (7 - column))));
         i++) {
        tempBoard = (1ll << (tile - (7 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // down-left
    for (int i = 1;
         i <= (column ^ (((7 - row) ^ column) & -((7 - row) < column))); i++) {
        tempBoard = (1ll << (tile + (7 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    // down-right
    for (int i = 1; i <= ((7 - column) ^ (((7 - row) ^ (7 - column)) &
                                          -((7 - row) < (7 - column))));
         i++) {
        tempBoard = (1ll << (tile + (9 * i)));
        if (tempBoard & allyBoard) {
            break;
        } else if (tempBoard & enemyBoard) {
            diagonalBoard = diagonalBoard | tempBoard;
            break;
        }
        diagonalBoard = diagonalBoard | tempBoard;
    }

    return diagonalBoard;
}

uint64_t CFBoard::getKnightPattern(int tile, bool color) {
    uint64_t knightPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    int knightPositions = (1 << 8) - 1;
    /*
    . 0 . 1 .
    2 . . . 3
    . . x . .
    4 . . . 5
    . 6 . 7 .
    */

    // may be able to be optimized
    switch (column) {
    case 0:
        knightPositions =
            knightPositions & ~((1 << 2) + (1 << 4) + 1 + (1 << 6));
        break;
    case 1:
        knightPositions = knightPositions & ~((1 << 2) + (1 << 4));
        break;
    case 6:
        knightPositions = knightPositions & ~((1 << 3) + (1 << 5));
        break;
    case 7:
        knightPositions =
            knightPositions & ~((1 << 3) + (1 << 5) + 2 + (1 << 7));
        break;
    default:
        break;
    }

    switch (row) {
    case 0:
        knightPositions = knightPositions & ~((1 << 4) - 1);
        break;
    case 1:
        knightPositions = knightPositions & ~((1 << 2) - 1);
        break;
    case 6:
        knightPositions = knightPositions & ~(((1 << 2) - 1) << 6);
        break;
    case 7:
        knightPositions = knightPositions & ~(((1 << 4) - 1) << 4);
        break;
    default:
        break;
    }

    for (int i = 0; i < 8; i++) {
        if ((knightPositions >> i) & 1) {
            switch (i) {
            case 0:
                knightPattern =
                    knightPattern | ((1ll << (tile - 17)) & ~allyBoard);
                break;
            case 1:
                knightPattern =
                    knightPattern | ((1ll << (tile - 15)) & ~allyBoard);
                break;
            case 2:
                knightPattern =
                    knightPattern | ((1ll << (tile - 10)) & ~allyBoard);
                break;
            case 3:
                knightPattern =
                    knightPattern | ((1ll << (tile - 6)) & ~allyBoard);
                break;
            case 4:
                knightPattern =
                    knightPattern | ((1ll << (tile + 6)) & ~allyBoard);
                break;
            case 5:
                knightPattern =
                    knightPattern | ((1ll << (tile + 10)) & ~allyBoard);
                break;
            case 6:
                knightPattern =
                    knightPattern | ((1ll << (tile + 15)) & ~allyBoard);
                break;
            case 7:
                knightPattern =
                    knightPattern | ((1ll << (tile + 17)) & ~allyBoard);
                break;
            }
        }
    }

    return knightPattern;
}

uint64_t CFBoard::getKingPattern(int tile, bool color) {
    uint64_t kingPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);

    if (column > 0) {
        kingPattern += (1 << (tile - 1));
    }
    if (row > 0) {
        kingPattern += (1 << (tile - 8));
    }
    if (column < 7) {
        kingPattern += (1 << (tile + 1));
    }
    if (row < 7) {
        kingPattern += (1 << (tile + 8));
    }
    if (column > 0 && row > 0) {
        kingPattern += (1 << (tile - 9));
    }
    if (column > 0 && row < 7) {
        kingPattern += (1 << (tile + 7));
    }
    if (column < 7 && row > 0) {
        kingPattern += (1 << (tile - 7));
    }
    if (column < 7 && row < 7) {
        kingPattern += (1 << (tile + 9));
    }
    return kingPattern;
}

uint64_t CFBoard::getPawnPattern(int tile, bool color) {
    uint64_t pawnPattern = 0;
    int column = tile & 7;
    int row = tile >> 3;
    uint64_t allyBoard = getColorBitBoard(color);
    uint64_t enemyBoard = getColorBitBoard(!color);

    if (color) { // black

        if (row == 7) { // just in case, should be useless in practice
            return pawnPattern;
        } else {
            if (!(((allyBoard | enemyBoard) >> (tile + 8)) & 1)) { // front
                pawnPattern += (1ll << (tile + 8));
                if ((row == 1) && (((allyBoard | enemyBoard) >> (tile + 16)) &
                                   1)) { // frontfront
                    pawnPattern += (1ll << (tile + 16));
                }
            }
            // diagonals
            if (column > 0) {
                if ((enPassantTarget == tile + 7) ||
                    (enemyBoard >> (tile + 7) & 1)) {
                    pawnPattern += (1ll << (tile + 7));
                }
            }
            if (column < 7) {
                if ((enPassantTarget == tile + 9) ||
                    (enemyBoard >> (tile + 9) & 1)) {
                    pawnPattern += (1ll << (tile + 9));
                }
            }
        }

    } else { // white

        if (row == 0) { // just in case, should be useless in practice
            return pawnPattern;
        } else {
            if (!(((allyBoard | enemyBoard) >> (tile - 8)) & 1)) { // front
                pawnPattern += (1ll << (tile - 8));
                if ((row == 6) && (((allyBoard | enemyBoard) >> (tile - 16)) &
                                   1)) { // frontfront
                    pawnPattern += (1ll << (tile - 16));
                }
            }
            // diagonals
            if (column > 0) {
                if ((enPassantTarget == tile - 9) ||
                    (enemyBoard >> (tile - 9) & 1)) {
                    pawnPattern += (1ll << (tile - 9));
                }
            }
            if (column < 7) {
                if ((enPassantTarget == tile - 7) ||
                    (enemyBoard >> (tile - 7) & 1)) {
                    pawnPattern += (1ll << (tile - 7));
                }
            }
        }
    }
}

uint64_t CFBoard::getLegalMoves(int pieceId, int tile) {
    bool color = pieceId & 1;
    uint64_t retBoard;
    switch (pieceId >> 1) {
    case 0: // pawn
        retBoard = getPawnPattern(tile, color);
    case 1: // knight
        retBoard = getKnightPattern(tile, color);
    case 2: // bishop
        retBoard = getDiagonals(tile, color);
    case 3: // rook
        retBoard = getCardinals(tile, color);
    case 4: // queen
        retBoard = getDiagonals(tile, color) | getCardinals(tile, color);
    case 5: // king
        retBoard = getKingPattern(tile, color);
    }
    uint64_t tmpBoard = retBoard;
    while (tmpBoard) {
        uint64_t lsb = tmpBoard & -tmpBoard;
        tmpBoard ^= lsb;
        int coordB = 63ll - __builtin_clzll(lsb);
        if (naiveCheckCheck(pieceId & 1, tile, coordB)) {
            retBoard ^= lsb;
        }
    }
    return retBoard;
}

// void CFBoard::naiveMovePiece(int starttile, int endtile) {}

// ========== Methods after this comment are implemented ==========

CFBoard::CFBoard() { // This is just the starter board.
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
    char pieceChar = '.';
    bool color = pieceId & 1;
    pieceId = pieceId >> 1;
    switch (pieceId) {
    case 0:
        pieceChar = 'P';
        break;
    case 1:
        pieceChar = 'N';
        break;
    case 2:
        pieceChar = 'B';
        break;
    case 3:
        pieceChar = 'R';
        break;
    case 4:
        pieceChar = 'Q';
        break;
    case 5:
        pieceChar = 'K';
        break;
    default:
        break;
    }
    if (color) {
        pieceChar = tolower(pieceChar);
    }
    return pieceChar
}

int CFBoard::pieceCharToId(char pieceChar) {
    int pieceId = NULL;
    switch (pieceChar) {
    case 'P':
        return 0;
    case 'p':
        return 1;
    case 'N':
        return 2;
    case 'n':
        return 3;
    case 'B':
        return 4;
    case 'b':
        return 5;
    case 'R':
        return 6;
    case 'r':
        return 7;
    case 'Q':
        return 8;
    case 'q':
        return 9;
    case 'K':
        return 10;
    case 'k':
        return 11;
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

uint64_t &CFBoard::getPieceBoardFromIndex(int boardIndex) {
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

    uint64_t &targetBoard = getPieceBoardFromIndex(pieceType);
    targetBoard = targetBoard | pieceBoard;

    if (color) {
        blackBoard = blackBoard | pieceBoard;
    } else {
        whiteBoard = whiteBoard | pieceBoard;
    }
}
void CFBoard::removePiece(int tile) {
    uint64_t antiPieceBoard = ~(1ll << tile);
    for (int pieceType = 0; pieceType < 6; pieceType++) {
        uint64_t &targetBoard = getPieceBoardFromIndex(pieceType);
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
        /*
        if (pieceId & 1) {
            pieceChar = tolower(pieceChar);
        }
        */
        repr += pieceChar;

        repr += " |";
        if ((tile + 1) % 8 == 0) {
            repr += "\n|";
        }
    }
    return repr;
}
