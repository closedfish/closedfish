#include "TestRandomMove.h"

TEST_CASE("SwitchEngine runs correctly with random moves", "[board]") {
	std::default_random_engine dre;
	dre.seed(time(NULL));
	TestRandomMove::init();
	SECTION("SwitchEngine against RandomMoveEngine") {
		CFBoard board;
		SwitchEngine swe(board, logger);
		RandomMoveEngine rme(board);
		REQUIRE(TestRandomMove::run(swe, rme));
	}
	SECTION("RandomMoveEngine against RandomMoveEngine") {
		CFBoard board;
		RandomMoveEngine rme1(board);
		RandomMoveEngine rme2(board);
		REQUIRE(TestRandomMove::run(rme1, rme2));
	}
}