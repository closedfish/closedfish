#include "TestChessHelper.h"

bool TestChessHelper::run(SwitchEngine &engine) {
	for (int i = 0; i < moves.size(); i++) {
		engine.processMove(moves[i]);
	}
	return true;
}