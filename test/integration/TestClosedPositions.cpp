#include "TestClosedPositions.h"

void testOnBoard(std::string boardFEN) {
	std::cerr << "running " << boardFEN << std::endl;
	SwitchEngine engine;
	SECTION(boardFEN) {
		CFBoard board(boardFEN);
		SwitchEngine swe(board, logger);
		REQUIRE(TestClosedPositions::run(swe));
	}
}

TEST_CASE("SwitchEngine runs correctly on closed positions", "[board]") {
	TestClosedPositions::init();
	// testOnBoard(
	// 		"rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");
	// testOnBoard(
	// 		"rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/2B1QB1R/3K3R w - - 0 1");
	// testOnBoard(
	// 		"rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R w - - 0 1");
	// testOnBoard(
	// 		"rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R b - - 0 1");
	// testOnBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
	// testOnBoard("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1BQ/R7/1B1K2RN w - - 0
	// 1"); testOnBoard("2k5/p7/qp5p/1qp3pP/B1qp1pP1/3npPN1/P3PB1R/3KQ2R w - - 0
	// 1"); testOnBoard("2k5/p7/qp5p/1qp3pP/2qp1pP1/3npPN1/P1B1PB1R/3KQ2R w - - 0
	// 1"); testOnBoard("2k5/p7/qp5p/1qp3pP/2qp1pP1/3npPN1/P1B1PB1R/3KQ2R w - - 0
	// 1"); testOnBoard("2k5/p7/Pp5p/1Pp3pq/2Pp1pq1/3PpnN1/P1B1PB1R/3KQ2R w - - 0
	// 1"); testOnBoard(
	// 		"2k5/p4q1q/Ppq1q1qp/1Ppq1qpq/2Ppqpq1/3PpnN1/P1B1PB1R/3KQ2R w - - 0 1");
	// testOnBoard(
	// 		"2kqqqqq/p2qqq2/Pp5p/1Pp3pP/2Pp1NP1/3PpPB1/P1B1P2R/3KQ2R b - - 0 1");
}