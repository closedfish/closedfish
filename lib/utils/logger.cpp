#include "logger.h"

namespace Closedfish {
Logger::Logger() {
	originalCoutRdbuf = std::cout.rdbuf();
	std::cout.rdbuf(stream.rdbuf());
	Logger::cout.rdbuf(originalCoutRdbuf);
}
Logger::~Logger() { std::cout.rdbuf(originalCoutRdbuf); }
} // namespace Closedfish

// Example for usage in main
int main_example() {
	Closedfish::Logger *logger =
			new Closedfish::Logger(); // now std::cout is hacked
	int n;
	std::cout << 60 << std::endl;
	logger->stream >> n;
	std::cout << "Hello, " << n << std::endl;
	std::string line;
	while (getline(logger->stream, line)) {
		std::cerr << line << std::endl;
	}
	delete logger; // now std::cout is unhacked
	std::cout << "this should be fine" << std::endl;
	return 0;
}