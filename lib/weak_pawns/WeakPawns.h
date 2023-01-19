#pragma once

#include <CFBoard.h>
#include <string>
#include <vector>

namespace WeakPawns {
int nbProtectingPawns(CFBoard &board, int &pTile);
int nbProtectingPiecesById(CFBoard &board, int &pTile, int boardId);
int nbProtectingPieces(CFBoard board, int tile);
bool isConnected(CFBoard &board, int &tile);
bool isPassed(CFBoard &board, int &tile);
bool isIsolated(CFBoard &board, int &tile);
uint64_t protectingTilesForPawns(CFBoard &board, int &pTile);
uint64_t protectingTilesForId(CFBoard &board, int pTile, int boardId);
uint64_t protectingTiles(CFBoard &board, int tile);
std::string ReprProtectingTiles(CFBoard board, int tile);
uint64_t getBoardProtectedByPawns(CFBoard board, bool color);
uint64_t blunderBoard(CFBoard board, bool color);
std::string ReprProtectedByPawn(CFBoard board, bool color);
}