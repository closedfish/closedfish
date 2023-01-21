#pragma once
#include <EngineWrapper.h>
#include <tuple>
#include <DFS1P.h>
// Waiting for Aarrya's implementation
// #include <Breakthrough2.h>
// #include <Breakthrough2.cpp>

class ClosedfishEngine : public Closedfish::ChessEngine {
public:
	ClosedfishEngine() : ChessEngine() {}
	Closedfish::Move getNextMove();
private:
	Closedfish::Move lastMove;
};