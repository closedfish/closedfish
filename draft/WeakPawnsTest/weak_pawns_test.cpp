#include <iostream>
using namespace std;

#include "../../lib/weak_pawns/WeakPawns.cpp"
#include "../../lib/weak_pawns/PieceMovements.cpp"
#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/board_implementation/CFBoard.cpp"

using namespace WeakPawns;

string toBinary(int n, int len)
{
    string binary;
    for (unsigned i = (1 << len - 1); i > 0; i = i / 2) {
        binary += (n & i) ? "1" : "0";
    }
 
    return binary;
}

int getBitFromPos(int i, int j) {
	return i*8 + j;
}

//creates a board manually 
CFBoard create_board_manually(bool color){

    int piece_board[8][8];
    uint64_t pawns = 0ll;
    uint64_t knights = 0ll;
    uint64_t bishops = 0ll;
    uint64_t rooks = 0ll;
    uint64_t queens = 0ll;
    uint64_t king = 0ll;
    uint64_t blacks = 0ll;
    uint64_t whites = 0ll;


    //initialization
    for(int i = 0; i<8; i++){
        for(int j = 0; j<8; j++){
            piece_board[i][j] = -1;
        }
    }

    //set up the target pawn
    int pi = 3;
    int pj = 2;
    piece_board[pi][pj] = color; //add 
    piece_board[pi-1][pj-1] = color; //add pawn
    piece_board[4][7] = color; //add pawn
    piece_board[pi+1][pj] = color; //add color

    //piece_board[pi + 2][pj + 2] = 5; //bishop
    //piece_board[pi + 2][pj - 2] = 5; //bishop
    //piece_board[pi + 3][pj] = 7; //rook
    //piece_board[pi + 2][pj - 1] = 3; //horse
    //piece_board[pi + 2][pj + 2] = 9; //queen
    //piece_board[pi][pj + 1] = 11; //king

    int bit;
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            if(piece_board[i][j] == 1 || piece_board[i][j] == 0){
                bit = getBitFromPos(i,j);
                pawns += (1ll << bit);
                if(piece_board[i][j] == 1){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
            if(piece_board[i][j] == 3 || piece_board[i][j] == 2){
                bit = getBitFromPos(i,j);
                knights += (1ll << bit);
                if(color){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
            if(piece_board[i][j] ==  5 || piece_board[i][j] == 4){
                bit = getBitFromPos(i,j);
                bishops += (1ll << bit);
                if(color){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
            if(piece_board[i][j] ==  7 || piece_board[i][j] == 6){
                bit = getBitFromPos(i,j);
                rooks += (1ll << bit);
                if(color){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
            if(piece_board[i][j] ==  9 || piece_board[i][j] == 8){
                bit = getBitFromPos(i,j);
                queens += (1ll << bit);
                if(color){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
            if(piece_board[i][j] ==  11 || piece_board[i][j] == 10){
                bit = getBitFromPos(i,j);
                king += (1ll << bit);
                if(color){
                    blacks += (1ll<<bit);
                }
                else{
                    whites += (1ll<<bit);
                }
            }
        }
    }
    
    //std::cout<<"knights "<<knights<<std::endl;
    CFBoard board = CFBoard(pawns, knights, bishops, rooks, queens, king, 0, 0, blacks, whites, 0);
    return board;

}

//creates a board from fen
CFBoard create_from_Fen(std::string fen){
    CFBoard board = CFBoard();
    board.fromFEN(fen);
    return board;
}

void test_pro_pieces(){
    
    bool color = 0;
    CFBoard board = create_board_manually(color);
    int pawn_i = 3;
    int pawn_j = 2;
    int tile = getBitFromPos(pawn_i, pawn_j);
    std::cout<<board.getRepr()<<std::endl;
    int pro_pieces = nbProtectingPieces(board,tile);
    std::cout<<pro_pieces<<std::endl;
    
}

void test_pro_tiles(){
    string fen = "rnbqkbnr/pppppppp/8/5N2/2P5/8/PP1PPPPP/RNBQKB1R b KQkq - 0 1";
    CFBoard board = create_from_Fen(fen);
    std::cout<<board.getRepr()<<std::endl;
    bool color = 0;
    int i = 3;
    int j = 2;
    int tile = i*8 + j;
    //uint64_t protect = prTiles(board, color, tile);
    //ReprProtectingTiles(board, color, tile);
}

int main(){
    bool color = 1;
    CFBoard board = create_board_manually(color);
    //std::cout<<ReprProtectedByPawn(board, color)<<std::endl;
    int i = 3;
    int j = 2;
    int tile = i*8 + j;
    std::cout<<board.getRepr()<<std::endl;
    //std::cout<<board.getReprLegalMove(1, tile)<<std::endl;
    //uint64_t c= (board, 0);
    int c = nbProtectingPieces(board, tile);
    std::cout<<c<<std::endl;
    return 0;

}
