#include "DFS1P.h"
#include <vector>
#include <queue>
#include <tuple>
#include <array>
#include <algorithm>
using std::cout;

void DFS1P::setBoardPointer(CFBoard* ptr) {
    if (ptr)
        currentBoard = ptr;
}

bool DFS1P::squareSafeFromOpponentPawns(const bool &currentTurn, const uint64_t& opponentPawnBoard, const int& row, const int &col) {
    // Black's turn
    if (currentTurn)
	    return row == 7 || ((col == 0 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row+1, col-1})))
				&& (col == 7 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row+1, col+1}))));
    // White's turn
    else
        return row == 0 || ((col == 0 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row-1, col-1})))
				&& (col == 7 || !isBitSet(opponentPawnBoard, Heatmap::posToTile({row-1, col+1}))));
}

//Distance between two squares with respect to a piece's movement using BFS
std::array<int, 64> DFS1P::distFromTileToTilesAsPiece(CFBoard& board, int halfPieceId, int startTile) {
    std::queue<int> q;
    std::array<int, 64> dist;
    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black
    uint64_t opponentPawnBoard = board.getPieceColorBitBoard(!currentTurn);

    // Initialize before BFS
    for (int i = 0; i < 64; i++)
        dist[i] = -1;
    dist[startTile] = 0;
    q.push(startTile);

    // BFS to find shortest distances from startTile
    while(!q.empty()) {
        int curTile = q.front();
        q.pop();

        // All next tiles possible
        uint64_t nextSquares = board.getLegalMoves(2*halfPieceId + currentTurn, curTile);
        for (int newTile: bitSetPositions(nextSquares)) {
            // Already visited
            if (dist[newTile] != -1) continue;
            // Square not empty
            if (board.getPieceFromCoords(newTile) != -1) continue;
            // Square unsafe
            if (!squareSafeFromOpponentPawns(currentTurn, opponentPawnBoard, newTile/8, newTile%8)) continue;

            dist[newTile] = dist[curTile] + 1;
            q.push(newTile);
        }
    }
    return dist;
}

int DFS1P::distFromHeatmap(CFBoard& board, int (&heatMap)[6][8][8]) {
    // Value to store the total distance which is the sum over pieces over tiles which are "hot" to that piece
    // of the heatmap value * the distance to the "hot" squares.
    int dist = 0;

    // Coefficient representing the value added to the distance when the desirable square is unreachable from current square
    const int COEFF_SEPARATED = 10; // adjustable

    bool currentTurn = board.getCurrentPlayer(); // 0: white, 1: black

    for (int halfPieceId = 0; halfPieceId < 6; halfPieceId++) {
        // Get current piece positions
        uint64_t pieceBoard = board.getPieceColorBitBoard(2*halfPieceId|currentTurn);
        std::vector<int> pieceTiles = bitSetPositions(pieceBoard);

        for (int startTile: pieceTiles) {
            // Find distances of all square from current square, with respect to the piece
            std::array<int, 64> distFromStart = distFromTileToTilesAsPiece(board, halfPieceId, startTile);
            
            for (int i = 0; i < 8; i++) {
                for (int j = 0; j < 8; j++) {
                    // Cold squares :cold_face:
                    if (heatMap[halfPieceId][i][j] == 0) continue;

                    int endTile = i*8+j;
                    // Unseparated square
                    if (distFromStart[endTile] != -1) {
                        dist += heatMap[halfPieceId][i][j] * distFromStart[endTile];
                    // Otherwise
                    } else {
                        dist += heatMap[halfPieceId][i][j] * COEFF_SEPARATED;
                    }
                }
            }
        }
    }
    return dist;
}

void DFS1P::DFS1pAux(CFBoard* currentBoard, int depth, int maxDepth, std::vector<std::tuple<int, int, float>> curLine, std::vector<std::vector<std::tuple<int, int, float>>>& possibleLines) {
    // Return if max depth is reached
    if (depth == maxDepth) {
        // Adding current line to possible lines
        possibleLines.push_back(curLine);
        return;
    }

    bool currentTurn = currentBoard->getCurrentPlayer(); // 0: white, 1: black

    for (int startTile = 0; startTile < 64; startTile++) {
        // Get piece at startTile, skip if it's empty or it contains opponent piece
        int pieceId = currentBoard->getPieceFromCoords(startTile);
        if (pieceId == -1 || pieceId&1 != currentTurn) continue;

        // Get all legal moves, represent it as a vector of possible end tiles
        uint64_t legalMoves =  currentBoard->getLegalMoves(pieceId, startTile);
        std::vector<int> endTiles = bitSetPositions(legalMoves);

        for (int endTile: endTiles) {
            // Avoid capturing
            if (currentBoard->getPieceFromCoords(endTile) != -1) continue;
            // Avoid unsafe moves
            if (!squareSafeFromOpponentPawns(currentTurn,
            currentBoard->getPieceColorBitBoard(!currentTurn), endTile/8, endTile%8)) continue;
            
            // Add the move to the current line
            curLine.push_back(std::make_tuple(startTile, endTile, 0.0));

            // Simulate the move
            currentBoard->movePiece(startTile, endTile);
            currentBoard->forceFlipTurn(); // skipping opponent's turn
            DFS1pAux(currentBoard, depth+1, maxDepth, curLine, possibleLines);
            
            // Unsimulate the move
            curLine.pop_back();
            currentBoard->undoLastMove();
            currentBoard->forceFlipTurn();
        }
    }
}

std::tuple<int, int, float> DFS1P::getNextMove() {
    int heatMap[6][8][8];
    memset(heatMap, 0, sizeof heatMap);
	uint64_t weakPawns = 1ll; // placeholder for finished weak pawns implementation

    // Build the heatmap
    Heatmap::addHeatMap(*currentBoard, heatMap, weakPawns);

    // CUSTOMIZABLE MAX DEPTH FOR DFS, CAPPED AT 4 CURRENTLY
    int maxDepth = 3;

    // Get all possible moves
    std::vector<std::tuple<int, int, float>> ansLine;
    std::vector<std::vector<std::tuple<int, int, float>>> possibleLines;
    int minDist = 1e9;

    // Put all possible lines into possibleLines
    DFS1pAux(currentBoard, 0, maxDepth, {}, possibleLines);

    // Iterate all lines
    for (auto line: possibleLines) {
        // Simulate all move in a line
        for (auto move: line) {
            currentBoard->movePiece(std::get<0>(move), std::get<1>(move));
            currentBoard->forceFlipTurn(); // one-person dfs so we assume the opponent doesn't move
        }

        // if (currentBoard->naiveCheckCheck(currentBoard->getCurrentPlayer())) {
        //     cout << "REDFLAG: SHOULDN'T BE IN CHECK\n";
        //     cout << "dist after: " << distFromHeatmap(*currentBoard, heatMap) << '\n';
        //     cout << currentBoard->getRepr();
        //     cout << currentBoard->toFEN();
        //     cout << "in check: " << currentBoard->naiveCheckCheck(currentBoard->getCurrentPlayer()) << '\n';
        // }

        // Check if the moves make us closer to the heatMap
        if (distFromHeatmap(*currentBoard, heatMap) < minDist) {
            minDist = distFromHeatmap(*currentBoard, heatMap);
            // If yes then update the most potential line
            ansLine = line;
        }

        // Undo all simulated moves after calculating
        for (auto move: line) {
            currentBoard->undoLastMove();
            currentBoard->forceFlipTurn();
        }
    }
    // cout << distFromHeatmap(*currentBoard, heatMap) << '\n';

    // Return the first move in the potential line
    return ansLine[0];
}

// int testDFS() {
// // int main() {
//     DFS1P algo;
//     CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP2N1/PpP5/1P4K1/RNBQ1B1R w - - 0 1");
//     // CFBoard board = CFBoard("rkqrbnnb/8/p5p1/Pp1p1pPp/1PpPpP1P/2P1P1N1/2B1QB1R/3K3R w - - 0 1"); // no open files, >= 2 free rows
// 	// CFBoard board = CFBoard("rkqr1nnb/4b3/8/p3p1p1/Pp1pPpPp/1PpP1P1P/R1P4N/1NKQBB1R w - - 0 1"); // no open files, 1 free rows, no chance of winning, 3 is better than 4 for some reasons
//     // CFBoard board = CFBoard("rnbqkbnr/pppppppp/8/8/8/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1");
//     // CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP4/PpP1BN1R/1P2QNK1/4RB2 w - - 0"); // wrong old naiveCheckCheck
//     // CFBoard board = CFBoard("rkq1bnnr/2b2p1p/4pPpP/3pP1P1/p1pP1B2/PpP3QR/1P1N1N2/R4BK1 w - - 0 1"); // wrong legal moves for bishop
//     // cout << board.naiveCheckCheck(0) << '\n';
//     // cout << board.getReprLegalMove(6, 47); 

//     algo.setBoardPointer(&board);
//     // cout << board.getRepr() << '\n';

//     // // Test dist between tiles
//     // for (int i = 0; i < 8; i++) {
//     //     for (int j = 0; j < 8; j++) {
//     //         int tile = i*8 + j;
//     //         cout << distFromTileToTileAsPiece(board, 2, 61, tile) << ' ';
//     //     }
//     //     cout << '\n';
//     // }

//     for (int i = 0; i < 15; i++) {
//         auto move = algo.getNextMove();
//         int startTile = std::get<0>(move), endTile = std::get<1>(move);
//         float eval = std::get<2>(move);
//         std::cout << startTile << ' ' << endTile << ' ' << eval << '\n';
//         board.movePiece(startTile, endTile);
//         board.forceFlipTurn(); // One person moving only
//         cout << board.getRepr() << '\n';
//     }
// }