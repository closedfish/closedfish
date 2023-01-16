#pragma once
#include <iostream>
#include <sstream>
#include <streambuf>

namespace Closedfish {
class Logger {
private:
	std::streambuf *originalCoutRdbuf;

public:
	std::stringstream stream;
	std::ostream cout = std::ostream(nullptr); // original cout
	Logger();
	~Logger();
};
} // namespace Closedfish