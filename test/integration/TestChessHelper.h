#pragma once

#include <ConnectUISAI.h>
#include <GeneralRegression.h>
#include <closenessAI.h>
#include <input.h>
#include <output.h>

#include <CFBoard.h>
#include <ClosedfishConnect.h>
#include <SwitchEngine.h>
#include <utils.h>

#include <cstdio>
#include <filesystem>
#include <fstream>
#include <functional>
#include <ios>
#include <iostream>
#include <vector>

#include "TestChessHelper.h"

class TestChessHelper {
public:
	TestChessHelper(std::vector<Closedfish::Move> moves) : moves(moves) {}
	bool run(SwitchEngine &engine);
	typedef void (*OnChange)(const Stockfish::UCI::Option &);
	static void opt(const Stockfish::UCI::Option &o) {
		Stockfish::Eval::NNUE::init();
	};
	static std::vector<Closedfish::Move> convertToMoves(std::ifstream &smithRaw) {
		std::string s;
		std::vector<Closedfish::Move> out;
		while (getline(smithRaw, s)) {
			if (s[0] == '[')
				continue;
			std::stringstream rowstream;
			rowstream << s;
			while (rowstream >> s) {
				if (s.back() == '.')
					continue;
				if (s[1] == '-')
					continue;
				out.push_back({parseAN(s.substr(0, 2)), parseAN(s.substr(2, 2)), 0.0});
			}
		}
		return out;
	}

	static void init() {
		Closedfish::Logger logger;
		char *argv[1];
		argv[0] = "";
		Stockfish::CommandLine::init(1, argv);
		Stockfish::UCI::init(Stockfish::Options);
		std::filesystem::path p = std::filesystem::path(CMAKE_SOURCE_DIR) /
															"external" / "stockfish" / "src" /
															(std::string)Stockfish::Options["EvalFile"];
		std::ifstream stream(p.string(), std::ios::binary);
		if (stream.fail()) {
			std::cerr << "[ERROR] EvalFile doesn't exist. Expected directory: "
								<< p.string() << std::endl;
			exit(-1);
		}
		stream.close();
		Stockfish::Options["EvalFile"]
				<< Stockfish::UCI::Option(p.string().c_str(), opt);
		Stockfish::Tune::init();
		Stockfish::PSQT::init();
		Stockfish::Bitboards::init();
		Stockfish::Position::init();
		Stockfish::Bitbases::init();
		Stockfish::Endgames::init();
		Stockfish::Threads.set(size_t(Stockfish::Options["Threads"]));
		Stockfish::Search::clear(); // After threads are up
		Stockfish::Eval::NNUE::init();
		Stockfish::Position::init();

		CFBoard board;
		SwitchEngine engine(board, &logger);

		srand(time(NULL));
	}

private:
	std::vector<Closedfish::Move> moves;
};