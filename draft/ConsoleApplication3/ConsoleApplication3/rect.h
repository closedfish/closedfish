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
