#include "test_to_fen.h"

TEST_CASE("Board to FEN is correct", "[board]") {
	REQUIRE(CFBoard().toFEN() ==
					"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
}