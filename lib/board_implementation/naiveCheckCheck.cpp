#include "CFBoard.h"

/**
 * @brief Helper function to check whether a position is inside the board or not
 *
 * @param row the specified row coordinate
 * @param col the specified col coordinate
 * @return true if it is valid
 * @return false if it is invalid
 */
inline bool isPositionValid(const int &row, const int &col) {
    return row >= 0 && row < 8 && col >= 0 && col < 8;
}

bool CFBoard::naiveCheckCheck(bool color, int coordA, int coordB) {
    uint64_t thisKingBoard = kingBoard & getColorBitBoard(color);
    if (!thisKingBoard)
        return false;
    uint64_t kingTile = 63ll - __builtin_clzll(thisKingBoard);
    // make sure kingTile is not coordA or coordB
    uint64_t otherBoard = getColorBitBoard(!color);
    if(coordA != -1) otherBoard &= ~(1ll << (uint64_t)coordA);
    if(coordB != -1) otherBoard &= ~(1ll << (uint64_t)coordB);
    uint64_t allBoard = whiteBoard | blackBoard;
    if(coordB != -1) allBoard |= (1ll << (uint64_t)coordB);
    if(coordA != -1) allBoard &= ~(1ll << (uint64_t)coordA);
    int kingRow = kingTile >> 3;
    int kingCol = kingTile & 7;
    // Check P, N
    int dx[] = {-1, -1, -1, -1, 1, 1, -2, -2, 2, 2};
    int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1, -1, 1};
    for (int i = 0; i < 2; i++) {
        // This loop should be unrolled
        // I hope the compiler does this for me
        int px = kingRow + dx[i];
        int py = kingCol + dy[i];
        uint64_t pTile = 1ll << (uint64_t)(px * 8 + py);
        if (isPositionValid(px, py) && (otherBoard & pawnBoard & pTile)) {
            return true;
        }
    }
    for (int i = 2; i < 10; i++) {
        // This loop should be unrolled
        // I hope the compiler does this for me
        int px = kingRow + dx[i];
        int py = kingCol + dy[i];
        uint64_t pTile = 1ll << (uint64_t)(px * 8 + py);
        if (isPositionValid(px, py) && (otherBoard & knightBoard & pTile)) {
            return true;
        }
    }
    // Check R
    for (int i = kingRow - 1; i >= 0; i--) {
        // Not sure if the compiler is smart enough to unroll this loop
        uint64_t pTile = 1ll << (uint64_t)(i * 8 + kingCol);
        if (otherBoard & rookBoard & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = kingRow + 1; i < 8; i++) {
        uint64_t pTile = 1ll << (uint64_t)(i * 8 + kingCol);
        if (otherBoard & rookBoard & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = kingCol - 1; i >= 0; i--) {
        uint64_t pTile = 1ll << (uint64_t)(kingRow * 8 + i);
        if (otherBoard & rookBoard & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = kingCol + 1; i < 8; i++) {
        uint64_t pTile = 1ll << (uint64_t)(kingRow * 8 + i);
        if (otherBoard & rookBoard & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    // Check B, Q
    for (int i = 1; isPositionValid(kingRow - i, kingCol - i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow - i) * 8 + (kingCol - i));
        if (otherBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow - i, kingCol + i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow - i) * 8 + (kingCol + i));
        if (otherBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow + i, kingCol - i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow + i) * 8 + (kingCol - i));
        if (otherBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow + i, kingCol + i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow + i) * 8 + (kingCol + i));
        if (otherBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if (allBoard & pTile) {
            break;
        }
    }
    return false;
}
