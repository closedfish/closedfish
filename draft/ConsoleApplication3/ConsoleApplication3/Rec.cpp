#include "rect.h"

square::square() {

};
square::square(COLOR color, cv::Rect rec, int x, int y) {
	this->color = color;
	this->rec = rec;
	this->position = cv::Point(x, y);
}