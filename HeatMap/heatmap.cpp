#include <iostream>
#include "../Board Implementation/CFBoard.cpp"
using namespace std;

void addPieceHeatMap(const char &piece, double (&ans)[8][8], int (&pawn_height)[8]) {
    if (piece == 'R' || piece == 'Q') {
        for (int j = 0; j < 8; j++) {
            for (int i = 0; i < pawn_height[j]; i++) {
                ans[i][j]++;
            }
        }
    }
}

void addHeatMap(CFBoard& board) {
    double ans[8][8];
    uint64_t one = 1;

    uint64_t mask_row[8], mask_col[8];
    uint64_t mask_first_row = (one<<8)-1;
    uint64_t mask_first_col = one + (one<<8) + (one<<16) + (one<<24) + (one<<32) + (one<<40) + (one<<48) + (one<<56);
    for (int i = 0; i < 8; i++) {
        mask_row[i] = mask_first_row<<(8*i);
        cout << (board.getPieceBitBoard('K') & mask_row[i]) << '\n';
    }
    for (int j = 0; j < 8; j++) {
        mask_col[j] = mask_first_col<<j;
    }

    bool current_turn = 0; // 0: white, 1: black
    int closed_rows = 8; // number of rows below the lowest pawn
    uint64_t pawn_board = board.getPieceBitBoard('P') & board.getColorBitBoard(current_turn);
    int pawn_height[8];
    for (int i = 0; i < 8; i++) pawn_height[i] = 8;
    for (int i = 0; i < 8; i++) {
        uint64_t pawn_row = (pawn_board & mask_row[i]) >> (8*i);
        if (!pawn_row) continue;
        if (current_turn) {
            for (int j = 0; j < 8; j++) {
                if ((pawn_row >> j)&1 && pawn_height[i] == 8) {
                    pawn_height[j] = i;
                }
            }
        } else {
            for (int j = 8; j > 0; j--) {
                if ((pawn_row >> j)&1 && pawn_height[i] == 8) {
                    pawn_height[j] = i;
                }
            }
        }
    }

    int no_bishops_same_pawn, no_bishops_diff_pawn, no_knights, no_rooks, no_queens;
    if (closed_rows >= 2) {
        for (int _ = 0; _ < no_rooks; _++)
            addPieceHeatMap('R', ans, pawn_height);
        for (int _ = 0; _ < no_queens; _++)
            addPieceHeatMap('Q', ans, pawn_height);
        

    } else {
        // To be added
    }
}

int main() {
    CFBoard board;
    addHeatMap(board);
    cout << board.getRepr();
}