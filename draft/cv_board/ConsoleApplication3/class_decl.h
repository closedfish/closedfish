#pragma once

#include "color_def.h"
#include <opencv2/core/types.hpp>
#include<tuple>
#include<iostream>
using namespace std;

class Square {
public:
	COLOR color;
	cv::Point position;
	cv::Rect rec;

	Square();
	Square(COLOR color, cv::Rect rec, int x, int y);
};
class Tile {//need it to stor chess coords, might merge with Square later on
public:
	char let;
	int cord;
	Tile();
	Tile(char, int);
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
	std::vector<Tile> Tiles;
	Board();
	Board(int,int,int,int);
};
