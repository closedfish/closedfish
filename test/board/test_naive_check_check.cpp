#include "test_naive_check_check.h"

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
