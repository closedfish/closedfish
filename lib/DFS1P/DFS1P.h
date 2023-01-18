#pragma once

#include "../board_implementation/CFBoard.h"
#include "../Algo/Algorithm.h"
#include "../heatmap/Heatmap.h"
#include "../heatmap/BitOperations.h"

class DFS1P : public ChessEngine {
public:
    void setBoardPointer(CFBoard* ptr);
    std::tuple<int, int, float> getNextMove();
    bool squareSafeFromOpponentPawns(const bool &currentTurn, const uint64_t& opponentPawnBoard, const int& row, const int &col);
    std::array<int, 64> distFromTileToTilesAsPiece(CFBoard& board, int halfPieceId, int startTile);
    int distFromHeatmap(CFBoard& board, int (&heatMap)[6][8][8]);
    void DFS1pAux(CFBoard* currentBoard, int depth, int maxDepth, std::vector<std::tuple<int, int, float>> curLine, std::vector<std::vector<std::tuple<int, int, float>>>& possibleLines);
};