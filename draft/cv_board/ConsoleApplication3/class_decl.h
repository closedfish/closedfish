#pragma once

#include "color_def.h"
#include <opencv2/core/types.hpp>

class Square {
public:
	COLOR color;
	cv::Point position;
	cv::Rect rec;

	Square();
	Square(COLOR color, cv::Rect rec, int x, int y);
};
class Player {
public:
	bool color;//if true, white, if false, black
	Player();
	Player(bool);
};
class Board {
public:
	int width;
	int height;
	int left;
	int right;
	int top;
	int bottom;
	Board();
	Board(int,int,int,int);
};
