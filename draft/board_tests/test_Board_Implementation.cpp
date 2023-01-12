#include "test_Board_Implementation.h"
using namespace std;

namespace CFBoardTester {

void test_pieceIdToChar(){
    int tests[12] = {0,1,2,3,4,5,6,7,8,9,10,11}; // every pieceId
    char out[12] = {'P','p','N','n','B','b','R','r','Q','q','K','k'};// every pieceChar
    std::cout << "test_pieceIdToChar" << std::endl;;
    bool TF = true
    for (int i=0; i < 12; i++) {
        assert(pieceCharToId(test[i]) == out[i])
        std::cout << "Test case " << i << "passed" << std::endl;
    }

    std::cout << "[Success!]" << std::endl;
}

void test_pieceCharToId(){
    char test[12] = {'P', 'p', 'N', 'n', 'B', 'b', 'R', 'r', 'Q', 'q', 'K', 'k'}; // every pieceChar
    int out[12] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}; // every pieceId
    std::cout << "test_pieceCarToId ";
    std::cout << "[";
    bool TF = true
    for (int i=0; i<12; i++) {
        if(pieceCharToId(test[i]) != out[i]) { // test pieceCharToId output to expected out
            cout << "Failure :(";
            TF = false;
        }
    }
    if (TF == true){
        cout << "Success!";
    }
    std::cout << "]" << std::endl;
}

void test_getColorBitBoard(){
    CFBoard board;  // Generate a default chess board
    
    std::cout << "test_getColorBitBoard : " << std::endl;
    // Test case 1: Check black board
    assert(board.getColorBitBoard(true) == board.whiteboard;
    std::cout << "Test case 1 passed" << std::endl;

    // Test case 2: Check white board
    assert(board.getColorBitBoard(false) == board.blackboard);
    std::cout << "Test case 2 passed" << std::endl;
    
    std::cout << "[Success!]" << std::endl;
    
}
void test_getPieceColorBitBoard(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = { // check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getPieceColorBitBoard ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getPieceBoardFromIndex(){
    std::cout << "test_getColorBitBoard : " << std::endl;
        
    CFBoard board;
        
    // Test case 1: Check pawn board
    assert(board.getPieceBoardFromIndex(0) == board.pawnBoard);
        std::cout << "Test case 1 passed" << std::endl;
    // Test case 2: Check knight board
    assert(board.getPieceBoardFromIndex(2) == board.knightBoard);
        std::cout << "Test case 2 passed" << std::endl;
    // Test case 3: Check bishop board
    assert(board.getPieceBoardFromIndex(4) == board.bishopBoard);
        std::cout << "Test case 3 passed" << std::endl;
    // Test case 4: Check rook board
    assert(board.getPieceBoardFromIndex(6) == board.rookBoard);
        std::cout << "Test case 4 passed" << std::endl;
    // Test case 5: Check queen board
    assert(board.getPieceBoardFromIndex(8) == board.queenBoard);
        std::cout << "Test case 5 passed" << std::endl;
    // Test case 6: Check king board
    assert(board.getPieceBoardFromIndex(10) == board.kingBoard);
        std::cout << "Test case 6 passed" << std::endl;
        
    
    std::cout << "[Success!]" << std::endl;
}

void test_getCurrentPlayer(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getCurrentPlayer ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getPieceFromCoords(){
    CFBoard board;
    int tests[19] = {0,1,2,3,4,5,6,7,11,23,55,56,57,58,59,60,61,62,63};
    int out[19] = {7,3,5,9,11,5,3,7,1,-1,0,6,2,4,8,10,4,2,6};
    std::cout << "test_getPieceFromCoords ";
    std::cout << "[";
    bool TF = true
    for (int i=0; i<19; i++) {
        if (board.getPieceFromCoords( tests[i] ) != out[i]) {
            cout << "Failure :(";
            TF = false;
        }
    }
    if (TF == true){
        cout << "Success!";
    }
    std::cout << "]" << std::endl;
}

void test_getBit(){
    CFBoard board;
    int tests_pos[10] = {0,1,2,3,4,5,6,7,23,55};
    int tests_piece[10] = {7,3,5,9,11,1,1,1,0,0}
    int out[10] = {1,1,1,1,1,0,0,0,0,1};
    std::cout << "test_getBit ";
    std::cout << "[";
    bool TF = true
    for (int i=0; i<10; i++) {
        if (board.getBit(tests_piece[i], tests_pos[i]) != out[i]) {
            cout << "Failure :(";
            TF = false;
        }
    }
    if (TF == true){
        cout << "Success!";
    }
    std::cout << "]" << std::endl;
}
void test_getMaterialCount(){
    CFBoard board;
    int tests[2] = {1,2}
    int out[2] = {16,16}
    std::cout << "test_getMaterialCount() ";
    std::cout << "[";
    bool TF = true
    for (int i=0; i<2; i++) {
        if (board.getMaterialCount(tests[i]) != out[i]) {
            cout << "Failure :(";
            TF = false;
        }
    }
    if (TF == true){
        cout << "Success!";
    }
    std::cout << "]" << std::endl;
}
void test_addPiece(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_addPiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_removePiece(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_removePiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_movePiece(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_movePiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_forceUndo(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_forceUndo ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getCardinals(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getCardinals ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getDiagonals(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getDiagonals ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getKnightPattern(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getKnightPattern ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getKingPattern(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getKingPattern ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getPawnPattern(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {// check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getPawnPattern ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getLegalMoves(){
    std::vector<std::tuple<bool, bool, CFBoard>> tests = { // check test cases
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    std::cout << "test_getLegalMoves ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}

void testNaiveCheckCheck() {
    std::vector<std::tuple<bool, bool, CFBoard>> tests = {
        std::make_tuple(false, false, CFBoard()),
        std::make_tuple(true, true,
                        CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/"
                                "RNBQKBNR w KQkq - 0 1"))};
    
    std::cout << "testNaiveCheckCheck ";
    std::cout << "[";
    for (auto test : tests) {
        bool isCheck = std::get<0>(test);
        if (isCheck == std::get<2>(test).naiveCheckCheck(std::get<1>(test))) {
            std::cout << "Success";
        } else {
            std::cout << "Failure";
        }
    }
    std::cout << "]" << std::endl;
}

void test_ToFEN() {
    std::vector<std::pair<std::string, CFBoard>> tests = {std::make_pair(
        std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
        CFBoard())};
    std::cout << "testToFEN ";
    std::cout << "[";
    for (auto test : tests) {
        std::string fen = test.first;
        std::string ret = test.second.toFEN();
        if (ret == fen) {
            std::cout << "Success";
        } else {
            std::cout << "Failure";
        }
    }
    std::cout << "]" << std::endl;
}

void test_FromFEN() {
    std::vector<std::pair<std::string, CFBoard>> tests = {std::make_pair(
        std::string("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"),
        CFBoard())};
    std::cout << "testFromFEN ";
    std::cout << "[";
    for (auto test : tests) {
        std::string fen = test.first;
        CFBoard board = test.second;
        if (board == CFBoard(fen)) {
            std::cout << "Success!";
        } else {
            std::cout << "Failure :(";
        }
    }
    std::cout << "]" << std::endl;
}
} // namespace CFBoardTester
  
