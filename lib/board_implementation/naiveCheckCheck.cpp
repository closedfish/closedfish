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

/**
 * @brief Checks if the white king is being checked by black or not
 *
 * @return true if it is checked
 * @return false if it is not checked
 */
bool CFBoard::naiveCheckCheck() {
    uint64_t whiteKingBoard = kingBoard & whiteBoard;
    uint64_t kingTile = whiteKingBoard & -whiteKingBoard;
    int kingRow = kingTile >> 3;
    int kingCol = kingTile & 7;
    // Check P, N
    int dx[] = {-1, -1, -1, -1, 1, 1, -2, -2, 2, 2};
    int dy[] = {-1, 1, -2, 2, -2, 2, -1, 1, -1, 1};
    for (int i = 0; i < 10; i++) {
        // This loop should be unrolled
        // I hope the compiler does this for me
        int px = kingRow + dx[i];
        int py = kingCol + dy[i];
        uint64_t pTile = 1ll << (uint64_t)(px * 8 + py);
        if (isPositionValid(px, py) && (blackBoard & pTile)) {
            return true;
        }
    }
    // Check R
    for (int i = kingRow - 1; i >= 0; i--) {
        // Not sure if the compiler is smart enough to unroll this loop
        uint64_t pTile = 1ll << (uint64_t)(i * 8 + kingCol);
        if (blackBoard & rookBoard & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = kingRow + 1; i < 8; i++) {
        uint64_t pTile = 1ll << (uint64_t)(i * 8 + kingCol);
        if (blackBoard & rookBoard & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = kingCol - 1; i >= 0; i--) {
        uint64_t pTile = 1ll << (uint64_t)(kingRow * 8 + i);
        if (blackBoard & rookBoard & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = kingCol + 1; i < 8; i++) {
        uint64_t pTile = 1ll << (uint64_t)(kingRow * 8 + i);
        if (blackBoard & rookBoard & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    // Check B, Q
    for (int i = 1; isPositionValid(kingRow - i, kingCol - i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow - i) * 8 + (kingCol - i));
        if (blackBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow - i, kingCol + i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow - i) * 8 + (kingCol + i));
        if (blackBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow + i, kingCol - i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow + i) * 8 + (kingCol - i));
        if (blackBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    for (int i = 1; isPositionValid(kingRow + i, kingCol + i); i++) {
        uint64_t pTile = 1ll << (uint64_t)((kingRow + i) * 8 + (kingCol + i));
        if (blackBoard & (bishopBoard | queenBoard) & pTile) {
            return true;
        } else if ((blackBoard | whiteBoard) & pTile) {
            break;
        }
    }
    return false;
}