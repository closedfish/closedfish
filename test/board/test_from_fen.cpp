#include "test_from_fen.h"

TEST_CASE("Board from FEN is correct", "[board]") {
	REQUIRE(CFBoard() ==
					CFBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"));
}