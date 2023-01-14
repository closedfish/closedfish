#include "test_naive_check_check.h"

TEST_CASE("Naive check check is correct", "[board]") {
	REQUIRE(CFBoard().naiveCheckCheck(false) == false);
	REQUIRE(
			CFBoard("rnbqkbnr/ppp1pppp/3p4/8/Q7/2P5/PP1PPPPP/RNBQKBNR w KQkq - 0 1")
					.naiveCheckCheck(true) == true);
}
