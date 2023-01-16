#pragma once
#include <EngineWrapper.h>
#include <tuple>

class ClosedfishEngine : public ChessEngine {
public:
	ClosedfishEngine() : ChessEngine() {}
	NextMove getNextMove();
};