#include "logger.h"
using namespace std;

namespace Closedfish {
Logger::Logger() {
	originalCoutRdbuf = cout.rdbuf();
	cout.rdbuf(stream.rdbuf());
}
Logger::~Logger() { cout.rdbuf(originalCoutRdbuf); }
} // namespace Closedfish

// Example for usage in main
int main_example() {
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