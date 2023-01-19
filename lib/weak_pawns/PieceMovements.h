#pragma once

#include "WeakPawns.h"

uint64_t getDangerousTiles(CFBoard board, bool color);
uint64_t getTilesThatCanEat(CFBoard &board, bool &color);
std::vector<int> CapturesFromLastMove(CFBoard &board, int lastMoveStart,
																			int lastMoveEnd);
bool isTileDangerous(CFBoard board, int tile);
int canPieceMoveOut(CFBoard &board, int tile, int pieceId);
uint64_t *getPieceMovements(CFBoard &board, bool &color, int tile);
int getLastMove(CFBoard lastBoard, CFBoard currentBoard, bool colorPlayed);