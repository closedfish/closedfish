#pragma once

#include "../../lib/board_implementation/CFBoard.h"
#include "../../lib/Algo/Algorithm.h"
#include "../../lib/heatmap/Heatmap.h"
#include "../../lib/heatmap/BitOperations.h"

class DFS1P : public ChessEngine {
public:
    void setBoardPointer(CFBoard* ptr);
    std::tuple<int, int, float> getNextMove();
};