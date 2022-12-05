#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
#include <string>
#include <string.h>
#include "../../lib/board_implementation/CFBoard.cpp"

bool validSquare(int i, int j) {
    return i >= 0 && i < 8 && j >= 0 && j < 8;
}
std::vector<int> tileToPos(int tile) {
    return {tile/8, tile%8};
}

int posToTile(std::vector<int> pos) {
    return pos[0] * 8 + pos[1];
}

uint64_t reverseBit(uint64_t v) {
    //credit to https://graphics.stanford.edu/~seander/bithacks.html#BitReverseObvious
    uint64_t r = v; // r will be reversed bits of v; first get LSB of v
    int s = sizeof(v) * CHAR_BIT - 1; // extra shift needed at end

    for (v >>= 1; v; v >>= 1)
    {   
        r <<= 1;
        r |= v & 1;
        s--;
    }
    r <<= s; // shift when v's highest bits are zero
    return r;
}

std::vector<int> tilesSetFromBoard(uint64_t board) {
    std::vector<int> tiles;
    while (board != 0) {
        int rightmost_tile = __builtin_ctz(board);
        tiles.push_back(rightmost_tile);
        board >>= rightmost_tile+1;
    }
    return tiles;
}

// std::vector<std::vector<int>> posSetFromBoard(const uint64_t &board) {
//     std::vector<std::vector<int>> pos;
//     std::vector<int> tiles = tilesSetFromBoard(board);
//     for (auto x: tiles) {
//         pos.push_back(posToTile(tiles));
//     }
//     return pos;
// }

void displayPawnBoard(const uint64_t& pawnBoard) { // for testing only
    uint64_t one = 1;
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << ((pawnBoard>>(i*8+j))&one);
        }
        std::cout << '\n';
    }
}

bool hasPawnOn(const uint64_t& pawn_board, const std::vector<int> &pos) {
    return (pawn_board>>posToTile(pos))&1;
}

void addHeatMap(CFBoard& board, int (&heat_map)[8][8], const uint64_t &weak_pawns) {
    uint64_t mask_row[8], mask_col[8]; // 1 for one column or one row, 0 otherwise
    mask_row[0] = (1LL<<8)-1;
    mask_col[0] = 1LL + (1LL<<8) + (1LL<<16) + (1LL<<24) + (1LL<<32) + (1LL<<40) + (1LL<<48) + (1LL<<56);
    for (int i = 1; i < 8; i++) {
        mask_row[i] = mask_row[i-1]<<8;
        mask_col[i] = mask_col[i-1]<<1;
    }

    bool current_turn = 1; // 0: white, 1: black
    uint64_t pawn_board = board.getPieceBitBoard('P') & board.getColorBitBoard(current_turn),
    knight_board = board.getPieceBitBoard('N') & board.getColorBitBoard(current_turn),
    bishop_board = board.getPieceBitBoard('B') & board.getColorBitBoard(current_turn),
    rook_board = board.getPieceBitBoard('R') & board.getColorBitBoard(current_turn),
    queen_board = board.getPieceBitBoard('Q') & board.getColorBitBoard(current_turn),
    king_board = board.getPieceBitBoard('K') & board.getColorBitBoard(current_turn);
    if (!current_turn) { // switch orientation from white to black for easier code
        pawn_board = reverseBit(pawn_board);
        knight_board = reverseBit(knight_board);
        bishop_board = reverseBit(bishop_board);
        rook_board = reverseBit(rook_board);
        queen_board = reverseBit(queen_board);
        king_board = reverseBit(king_board);
    }

    int pawn_height[8] = {8, 8, 8, 8, 8, 8, 8, 8};
    // displayPawnBoard(pawn_board);
    for (int i = 0; i < 8; i++) { 
        uint64_t pawn_row = (pawn_board & mask_row[i]) >> (8*i);
        if (!pawn_row) continue;
        for (int j = 0; j < 8; j++) {
            if ((pawn_row >> j)&1 && pawn_height[j] == 8) {
                pawn_height[j] = i;
            }
        }
    }
    std::vector<int> open_file;
    for (int j = 0; j < 8; j++) {
        if (pawn_height[j] == 8) {
            open_file.push_back(j);
        }
    }
    // for (int j = 0; j < 8; j++)
    //     std::cout << pawn_height[j] << ' ';
    // std::cout << '\n';
    int closed_rows = (*std::min_element(pawn_height, pawn_height+8)) - 1; // number of rows below the lowest pawn
    // std::cout << closed_rows << '\n';

    if (open_file.size() > 0) { // there exists open files
        uint64_t opponent_pawn_board = board.getPieceBitBoard('P') & board.getColorBitBoard(!current_turn);
        if (!current_turn) {
            opponent_pawn_board = reverseBit(opponent_pawn_board);
        }
        int no_rooks = __builtin_popcount(rook_board),
            no_queens = __builtin_popcount(queen_board),
            no_bishops = __builtin_popcount(bishop_board),
            no_knights = __builtin_popcount(knight_board);
        for (int file: open_file) {
            int max_pawn_height = std::min(file > 0 ? pawn_height[file-1] : 8, file < 7 ? pawn_height[file+1] : 8);
            // Rooks and Queens move to open file
            for (int i = 0; i < max_pawn_height; i++) {
                if ((file == 0 || !hasPawnOn(opponent_pawn_board, {i+1, file-1}))
                && (file == 8 || !hasPawnOn(opponent_pawn_board, {i+1, file+1}))) // no opposing pawns
                    heat_map[i][file] += no_rooks + no_queens;
            }
            // Knights move to square near open file, defending the nearby pawns and pieces
            int di[8] = {-1, -2, -2, -1, 1, 2, 2, 1};
            int dj[8] = {2, 1, -1, -2, -2, -1, 1, 2};
            for (int i = 0; i < max_pawn_height; i++) {
                for (int k = 0; k < 8; k++) {
                    if (validSquare(i+di[k], file+dj[k])) {
                        heat_map[i+di[k]][file+dj[k]] += no_knights;
                    }
                }
            }
            if (file >= 1 && pawn_height[file-1] < 8) {
                for (int k = 0; k < 8; k++) {
                    if (validSquare(pawn_height[file-1]+di[k], file-1+dj[k])) {
                        heat_map[pawn_height[file-1]+di[k]][file-1+dj[k]] += no_knights;
                    }
                }
            }
            if (file <= 6 && pawn_height[file+1] < 8) {
                for (int k = 0; k < 8; k++) {
                    if (validSquare(pawn_height[file+1]+di[k], file+1+dj[k])) {
                        heat_map[pawn_height[file+1]+di[k]][file+1+dj[k]] += no_knights;
                    }
                }
            }
            // Calculate how many bishop of the same color with pawn structure
            int pawn_color_cnt[2] = {0, 0};
            for (int j = 0; j < 8; j++) {
                if (pawn_height[j] != 8) {
                    pawn_color_cnt[(j+pawn_height[j])%2]++;
                }
            }
            bool pawn_structure_color_main;
            if (pawn_color_cnt[0] > pawn_color_cnt[1])
                pawn_structure_color_main = 0;
            else
                pawn_structure_color_main = 1;

            int no_bishops_color[2] = {0, 0};
            std::vector<int> bishop_tiles = tilesSetFromBoard(bishop_board);
            for (int tile: bishop_tiles) {
                no_bishops_color[tile%2 == 0]++;
            }
            // Bishops same color with our pawn structure
            // if (tile > 1) { // Protect left pawn
            //     for (int i = 1; validSquare(pawn_height[tile-1]-i, tile-i); i++) {
            //         heat_map[pawn_height[tile-1]-i][tile-i] += 1;
            //     }
            //     for (int i = 1; validSquare(pawn_height[tile+1]))
            // }
            // for (int i = 0; i < max_pawn_height; i++) {
            //     for (int j = pawn_structure_color_main; )
            //     heat_map += no_bishops_color[pawn_structure_color_main];
            // }
            // // Bishops diff color with our pawn structure
            // for (int i = 0; i < max_pawn_height; i++) {
            //     heat_map += no_bishops_color[!pawn_structure_color_main];
            // }
        }
    } else { // No open files
        if (closed_rows >= 2) { // 2 free rows, knights, rooks and queens are essentially able to go anywhere
            // Rooks and Queens
            std::vector<int> weak_pawns_file = {}; // placeholder waiting for weak pawns implementation
            for (int file: weak_pawns_file) {
            }

        } else {

            // To be added
        }
    }
}

int main() {
    int heat_map[8][8];
    memset(heat_map, 0, sizeof heat_map);

    CFBoard board;
    // std::cout << board.getRepr();
    uint64_t weak_pawns = (1<<8)-1; // placeholder for finished weak pawns implementation
    addHeatMap(board, heat_map, weak_pawns);
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            std::cout << heat_map[i][j] << ' ';
        }
        std::cout << '\n';
    }
}