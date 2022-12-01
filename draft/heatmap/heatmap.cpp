#include <iostream>
#include <cstring>
#include "../Board Implementation/CFBoard.cpp"

void addPieceHeatMap(const char &piece, int (&heat_map)[8][8], int (&pawn_height)[8], const bool &current_turn) {
    if (piece == 'N' || piece == 'R' || piece == 'Q') {
        if (current_turn) {
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < pawn_height[j]; i++) {
                    heat_map[i][j]++;
                }
            }
        }
        else {
            for (int j = 0; j < 8; j++) {
                for (int i = 7; i > 7 - pawn_height[j] + 1; i--) {
                    heat_map[i][j]++;
                }
            }
        }
    }
}

void addHeatMap(CFBoard& board, int (&heat_map)[8][8]) {
    uint64_t one = 1;

    uint64_t mask_row[8], mask_col[8];
    uint64_t mask_first_row = (one<<8)-1;
    uint64_t mask_first_col = one + (one<<8) + (one<<16) + (one<<24) + (one<<32) + (one<<40) + (one<<48) + (one<<56);
    for (int i = 0; i < 8; i++) {
        mask_row[i] = mask_first_row<<(8*i);
    }
    for (int j = 0; j < 8; j++) {
        mask_col[j] = mask_first_col<<j;
    }

    bool current_turn = 1; // 0: white, 1: black
    uint64_t pawn_board = board.getPieceBitBoard('P') & board.getColorBitBoard(current_turn);
    int pawn_height[8] = {8, 8, 8, 8, 8, 8, 8, 8};
    if (current_turn) {
        for (int i = 0; i < 8; i++) {
            uint64_t pawn_row = (pawn_board & mask_row[i]) >> (8*i);
            if (!pawn_row) continue;
            for (int j = 0; j < 8; j++) {
                if ((pawn_row >> j)&1 && pawn_height[j] == 8) {
                    pawn_height[j] = i;
                }
            }
        }
    } else {
        for (int i = 7; i >= 0; i--) {
            uint64_t pawn_row = (pawn_board & mask_row[i]) >> (8*i);
            if (!pawn_row) continue;
            for (int j = 0; j < 8; j++) {
                if ((pawn_row >> j)&1 && pawn_height[j] == 8) {
                    pawn_height[j] = 8-i;
                }
            }
        }
    }

    // for (int j = 0; j < 8; j++)
    //     std::cout << pawn_height[j] << ' ';
    // std::cout << '\n';

    int closed_rows = 8; // number of rows below the lowest pawn
    for (int i = 0; i < 8; i++)
        closed_rows = std::min(closed_rows, pawn_height[i]-1);
    // std::cout << closed_rows << '\n';

    int no_bishops_same_pawn, // to be
    no_bishops_diff_pawn,     // added
    no_knights = __builtin_popcountll(board.getPieceBitBoard('N') & board.getColorBitBoard(current_turn)),
    no_rooks = __builtin_popcountll(board.getPieceBitBoard('R') & board.getColorBitBoard(current_turn)),
    no_queens = __builtin_popcountll(board.getPieceBitBoard('Q') & board.getColorBitBoard(current_turn));
    // std::cout << no_knights << ' ' << no_rooks << ' ' << no_queens << '\n';
    if (closed_rows >= 2 || 1) { // 2 free rows, knights, rooks and queens are essentially able to go anywhere
        for (int _ = 0; _ < no_knights; _++)
            addPieceHeatMap('N', heat_map, pawn_height, current_turn);
        for (int _ = 0; _ < no_rooks; _++)
            addPieceHeatMap('R', heat_map, pawn_height, current_turn);
        for (int _ = 0; _ < no_queens; _++)
            addPieceHeatMap('Q', heat_map, pawn_height, current_turn);
        // To be added
    } else {

        // To be added
    }
}

int main() {
    int heat_map[8][8];
    memset(heat_map, 0, sizeof heat_map);

    CFBoard board;
    // std::cout << board.getRepr();
    addHeatMap(board, heat_map);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << heat_map[i][j] << ' ';
        }
        std::cout << '\n';
    }
}