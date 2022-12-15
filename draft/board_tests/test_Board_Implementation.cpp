#include "test_Board_Implementation.h"
using namespace std;

namespace CFBoardTester {

void test_pieceIdToChar();
void test_pieceCharToId();
void test_getColorBitBoard();
void test_getPieceColorBitBoard();
void test_getPieceBoardFromIndex();
void test_getCurrentPlayer();
void test_getPieceFromCoords();
void test_getBit();
void test_getMaterialCount();
void test_addPiece();
void test_removePiece();
void test_movePiece();
void test_forceUndo();
void test_getCardinals();
void test_getDiagonals();
void test_getKnightPattern();
void test_getKingPattern();
void test_getPawnPattern();
void test_getLegalMoves();
void test_naiveCheckCheck();
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
            std::cout << "P";
        } else {
            std::cout << "-";
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
            std::cout << "P";
        } else {
            std::cout << "-";
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
            std::cout << "P";
        } else {
            std::cout << "-";
        }
    }
    std::cout << "]" << std::endl;
}
} // namespace CFBoardTester
  
