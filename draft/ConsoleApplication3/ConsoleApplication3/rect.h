#pragma once

#include "col.h"
#include <opencv2/core/types.hpp>

class square {
public:
	COLOR color;
	cv::Point position;
	cv::Rect rec;

	square();
	square(COLOR color, cv::Rect rec, int x, int y);
};
class PLAYER {
public:
	bool color;//if true, white, if false, black
	PLAYER();
	PLAYER(bool);
};
class board {
public:
	int width;
	int height;
	int left;
	int right;
	int top;
	int bottom;
	board();
	board(int,int,int,int);
};
