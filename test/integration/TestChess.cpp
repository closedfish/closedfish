#include "TestChessHelper.h"

void testOnGame(std::string game) {
	std::ifstream in((std::filesystem::path(CMAKE_SOURCE_DIR) / "test" /
										"integration" / "data" / (game + ".parsed.txt"))
											 .string()
											 .c_str());
	std::vector<Closedfish::Move> moves = TestChessHelper::convertToMoves(in);
	SwitchEngine engine;
	SECTION(game) {
		CFBoard board;
		for (auto move : moves) {
			board.movePiece(std::get<0>(move), std::get<1>(move));
		}
		REQUIRE(true); // replace with endgame check
	}
}

TEST_CASE("CFBoard simulates games of chess correctly", "[board]") {
	TestChessHelper::init();
	testOnGame("balogh_keres_1937");
	testOnGame("castaldi_tartakower_1937");
	testOnGame("euwe_lilienthal_1937");
	testOnGame("keres_reshevsky_1937");
	testOnGame("ozols_reid_1937");
	testOnGame("schmidt_nowarra_1941");
}