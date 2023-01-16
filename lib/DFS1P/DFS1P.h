#pragma once

#include "../board_implementation/CFBoard.h"
#include "../Algo/Algorithm.h"
#include "../heatmap/Heatmap.h"
#include "../heatmap/BitOperations.h"

class DFS1P : public ChessEngine {
public:
    void setBoardPointer(CFBoard* ptr);
    std::tuple<int, int, float> getNextMove();
};