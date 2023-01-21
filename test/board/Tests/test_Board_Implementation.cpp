#include "test_Board_Implementation.h"
using namespace std;


TEST_CASE("PieceId to Char is correct", "[board]") {
    REQUIRE(pieceCharToId(0) == 'P');
    REQUIRE(pieceCharToId(1) == 'p');
    REQUIRE(pieceCharToId(2) == 'N');
    REQUIRE(pieceCharToId(3) == 'n');
    REQUIRE(pieceCharToId(4) == 'B');
    REQUIRE(pieceCharToId(5) == 'b');
    REQUIRE(pieceCharToId(6) == 'R');
    REQUIRE(pieceCharToId(7) == 'r');
    REQUIRE(pieceCharToId(8) == 'Q');
    REQUIRE(pieceCharToId(9) == 'q');
    REQUIRE(pieceCharToId(10) == 'K');
    REQUIRE(pieceCharToId(11) == 'k');
}

TEST_CASE("Char to PieceId is correct", "[board]") {
    REQUIRE(pieceCharToId('P') == 0);
    REQUIRE(pieceCharToId('p') == 1);
    REQUIRE(pieceCharToId('N') == 2);
    REQUIRE(pieceCharToId('n')) == 3);
    REQUIRE(pieceCharToId('B') == 4);
    REQUIRE(pieceCharToId('b') == 5);
    REQUIRE(pieceCharToId('R') == 6);
    REQUIRE(pieceCharToId('r') == 7);
    REQUIRE(pieceCharToId('Q') == 8);
    REQUIRE(pieceCharToId('q') == 9);
    REQUIRE(pieceCharToId('K') == 10);
    REQUIRE(pieceCharToId('k') == 11);
}

TEST_CASE("Get color BitBoard", "[board]") {
    REQUIRE(CFBoard().getColorBitBoard(true) == ((1ll << 16) - 1) << 48);
    REQUIRE(CFBoard().getColorBitBoard(false) == (1ll << 16) - 1);
}

TEST_CASE("Get Piece and Color BitBoard", "[board]") {
    REQUIRE(CFBoard().getColorBitBoard(true) == ((1ll << 16) - 1) << 48);
    REQUIRE(CFBoard().getColorBitBoard(false) == (1ll << 16) - 1);
}

TEST_CASE("Get Piece Board from Index", "[board]") {
    REQUIRE(CFBoard().getPieceBoardFromIndex(0) == (((1ll << 8) - 1) << 48) + (((1ll << 8) - 1) << 8));
    REQUIRE(CFBoard().getPieceBoardFromIndex(2) == (1ll << 1) + (1ll << 6) + (1ll << 57) + (1ll << 62));
    REQUIRE(CFBoard().getPieceBoardFromIndex(4) == (1ll << 2) + (1ll << 5) + (1ll << 58) + (1ll << 61));
    REQUIRE(CFBoard().getPieceBoardFromIndex(6) == (1ll << 0) + (1ll << 7) + (1ll << 56) + (1ll << 63));
    REQUIRE(CFBoard().getPieceBoardFromIndex(8) == (1ll << 3) + (1ll << 59));
    REQUIRE(CFBoard().getPieceBoardFromIndex(10) == (1ll << 4) + (1ll << 60));
}

TEST_CASE("Get Current Player", "[board]") {
    REQUIRE(CFBoard().getCurrentPlayer() == 0);
}

TEST_CASE("Get Piece from Coords", "[board]") {
    REQUIRE(CFBoard().getPieceFromCoords(0) == 7);
    REQUIRE(CFBoard().getPieceFromCoords(1) == 3);
    REQUIRE(CFBoard().getPieceFromCoords(2) == 5);
    REQUIRE(CFBoard().getPieceFromCoords(3) == 9);
    REQUIRE(CFBoard().getPieceFromCoords(4) == 11);
    REQUIRE(CFBoard().getPieceFromCoords(5) == 5);
    REQUIRE(CFBoard().getPieceFromCoords(6) == 3);
    REQUIRE(CFBoard().getPieceFromCoords(7) == 7);
    REQUIRE(CFBoard().getPieceFromCoords(11) == 1);
    REQUIRE(CFBoard().getPieceFromCoords(23) == -1);
    REQUIRE(CFBoard().getPieceFromCoords(55) == 0);
    REQUIRE(CFBoard().getPieceFromCoords(56) == 6);
    REQUIRE(CFBoard().getPieceFromCoords(57) == 2);
    REQUIRE(CFBoard().getPieceFromCoords(58) == 4);
    REQUIRE(CFBoard().getPieceFromCoords(59) == 8);
    REQUIRE(CFBoard().getPieceFromCoords(60) == 10);
    REQUIRE(CFBoard().getPieceFromCoords(61) == 4);
    REQUIRE(CFBoard().getPieceFromCoords(62) == 2);
    REQUIRE(CFBoard().getPieceFromCoords(63) == 6);

}

TEST_CASE("Get Bit", "[board]") {
    REQUIRE(CFBoard().getBit(0, 7) == 1);
    REQUIRE(CFBoard().getBit(1, 3) == 1);
    REQUIRE(CFBoard().getBit(2, 5) == 1);
    REQUIRE(CFBoard().getBit(3, 9) == 1);
    REQUIRE(CFBoard().getBit(4, 11) == 1);
    REQUIRE(CFBoard().getBit(5, 1) == 0);
    REQUIRE(CFBoard().getBit(6, 1) == 0);
    REQUIRE(CFBoard().getBit(7, 1) == 0);
    REQUIRE(CFBoard().getBit(23, 0) == 0);
    REQUIRE(CFBoard().getBit(55, 0) == 1);


}

TEST_CASE("Get Material Count", "[board]") {
    REQUIRE(CFBoard().getMaterialCount(1) == 16);
    REQUIRE(CFBoard().getMaterialCount(2) == 16);

}

TEST_CASE("Get Knight Pattern", "[board]") {
    //

}

TEST_CASE("Get King Pattern", "[board]") {
    //

}

TEST_CASE("Get Pawn Pattern", "[board]") {
    //

}








TEST_CASE("Naive check check is correct", "[board]") {
    REQUIRE(CFBoard().naiveCheckCheck(false) == false);
    REQUIRE(
            CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1")
                    .naiveCheckCheck(true) == true);
    REQUIRE(
            CFBoard(
                    "rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP3K1/1P6/RNBQ1B1R w - - 0 1")
                    .naiveCheckCheck(0, 38, 55) == false);
    REQUIRE(
            CFBoard(
                    "rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1K2/PpP1N3/1P6/RNBQ1BR1 w - - 0 1")
                    .naiveCheckCheck(0, -1, -1) == false);
}

TEST_CASE("Board to FEN is correct", "[board]") {
    REQUIRE(CFBoard().toFEN() ==
                    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}

TEST_CASE("Board from FEN is correct", "[board]") {
    REQUIRE(CFBoard() ==
                    CFBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
}



           

}
void test_getKnightPattern() {
    std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
        std::make_tuple(0, false, CFBoard(), 0x20400),
        std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
    };

    std::cout << "test_getKnightPattern ";
    std::cout << "[";
    for (auto test : tests) {
        int tile = std::get<0>(test);
        bool color = std::get<1>(test);
        CFBoard board = std::get<2>(test);
        uint64_t expected = std::get<3>(test);
        uint64_t result = board.getKnightPattern(tile, color);
        if (result != expected) {
            std::cout << "FAILED";
            return;
        }
    }
    std::cout << "PASSED";
    std::cout << "]" << std::endl;
}
void test_getKingPattern() {
    std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
        std::make_tuple(0, false, CFBoard(), 0x302),
        std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x202)
    };

    std::cout << "test_getKingPattern ";
    std::cout << "[";
    for (auto test : tests) {
        int tile = std::get<0>(test);
        bool color = std::get<1>(test);
        CFBoard board = std::get<2>(test);
        uint64_t expected = std::get<3>(test);
        uint64_t result = board.getKingPattern(tile, color);
        if (result != expected) {
            std::cout << "FAILED";
            return;
        }
    }
    std::cout << "PASSED";
    std::cout << "]" << std::endl;
}
void test_getPawnPattern() {
    std::vector<std::tuple<uint64_t, bool, int, CFBoard>> tests = {
        std::make_tuple(0x000000000000ff00, true, 0, CFBoard()),
        std::make_tuple(0x000000000000ff00, true, 8, CFBoard()),
        std::make_tuple(0x000000000000ff00, false, 56, CFBoard()),
        std::make_tuple(0x00000000000000ff, false, 63, CFBoard()),
        std::make_tuple(0x0020000000000000, true, 48,
                        CFBoard("8/8/8/8/8/8/4P3/8 b - - 0 1"))};

    std::cout << "test_getPawnPattern ";
    std::cout << "[";

    for (auto test : tests) {
        uint64_t expected = std::get<0>(test);
        bool color = std::get<1>(test);
        int tile = std::get<2>(test);
        CFBoard board = std::get<3>(test);

        uint64_t actual = board.getPawnPattern(tile, color);
        if (expected == actual) {
            std::cout << ".";
        } else {
            std::cout << "F";
        }
    }
    std::cout << "]" << std::endl;
}


/*void test_getLegalMoves(){
    CFBoard board;
    std::cout << "test_getLegalMoves ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}


void test_addPiece(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_addPiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
   
}
void test_removePiece(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_removePiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_movePiece(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_movePiece ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_forceUndo(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_forceUndo ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getCardinals(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_getCardinals ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }
}
void test_getDiagonals(){
        std::vector<std::tuple<int, bool, CFBoard, uint64_t>> tests = {
            std::make_tuple(0, false, CFBoard(), 0x20400),
            std::make_tuple(1, true, CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1"), 0x10100)
        };
    std::cout << "test_getDiagonals ";
    std::cout << "[";
    for (auto test : tests) {
        //implement
    }*/
