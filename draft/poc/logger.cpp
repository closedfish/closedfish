#include "../../external/stockfish/src/misc.h"
#include <iostream>
#include <sstream>
using namespace std;

namespace Closedfish {
class Logger {
  private:
    streambuf *originalCoutRdbuf;

  public:
    stringstream stream;
    Logger() {
        originalCoutRdbuf = cout.rdbuf();
        cout.rdbuf(stream.rdbuf());
    }
    ~Logger() { cout.rdbuf(originalCoutRdbuf); }
};
} // namespace Closedfish

int main() {
    Closedfish::Logger *logger = new Closedfish::Logger(); // now cout is hacked
    int n;
    cout << 60 << endl;
    logger->stream >> n;
    cout << "Hello, " << n << endl;
    string line;
    while (getline(logger->stream, line)) {
        cerr << line << endl;
    }
    delete logger; // now cout is unhacked
    cout << "this should be fine" << endl;
    return 0;
}