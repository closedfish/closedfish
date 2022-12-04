#include "rect.h"

square::square() {

};
square::square(COLOR color, cv::Rect rec, int x, int y) {
	this->color = color;
	this->rec = rec;
	this->position = cv::Point(x, y);
}
board::board() {
	this->left = 10000;
	this->top = 10000;
	this->bottom = 0;
	this->right = 0;

}
board::board(int left, int right, int top, int bottom) {
	this->left = left;
	this->right = right;
	this->top = top;
	this->bottom = bottom;
}
PLAYER::PLAYER() {
	this->color = true;
}
PLAYER::PLAYER(bool color) {
	this->color = color;
}