#pragma once
#include <EngineWrapper.h>
#include <tuple>

class ClosedfishEngine : public Closedfish::ChessEngine {
public:
	ClosedfishEngine() : ChessEngine() {}
	Closedfish::Move getNextMove();
};