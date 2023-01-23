
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>

/// Copied from EngineWrapper.h
namespace Closedfish {
// Gives you the information on next move
typedef std::tuple<int, int, float> Move;
} // namespace Closedfish

/// Copied from utils.h
/**
 * @brief Convert algebraic notation to int on 64
 *
 * @param str algebraic notation
 * @return int position (8 * row + col)
 */
int parseAN(std::string str) {
	int row = 7 - (str[1] - '1');
	int col = str[0] - 'a';
	return row * 8 + col;
}

std::vector<Closedfish::Move> convertToMoves(std::ifstream &smithRaw) {
	std::string s;
	std::vector<Closedfish::Move> out;
	while (getline(smithRaw, s)) {
		if (s[0] == '[')
			continue;
		std::stringstream rowstream;
		rowstream << s;
		while (rowstream >> s) {
			if (s.back() == '.') {
				continue;
			}
			std::cout << "DBG " << s << std::endl;
			out.push_back({parseAN(s.substr(0, 2)), parseAN(s.substr(2, 2)), 0.0});
		}
	}
	return out;
}

std::string getRepr(std::vector<Closedfish::Move> moves) {
	std::string out;
	out.push_back('{');
	for (auto move : moves) {
		out += "{";
		out += std::to_string(std::get<0>(move));
		out += ",";
		out += std::to_string(std::get<1>(move));
		out += ",0.0},";
	}
	out.back() = '}';
	return out;
}

int main(int argc, char *argv[]) {
	std::ifstream in(argv[1]);
	std::vector<Closedfish::Move> moves = convertToMoves(in);
	std::cout << getRepr(moves) << std::endl;
	return 0;
}