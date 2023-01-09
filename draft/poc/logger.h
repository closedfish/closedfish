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
    Logger();
    ~Logger();
};
} // namespace Closedfish