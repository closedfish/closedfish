# stress_tests

This contains the utilities to make a stress test using normal chess matches as inputs.

- `pgn-extract.exe` converts the pgn game notation to smith notation (which is easier for us to use).
- `reader.cpp` reads the parsed smith notation and outputs a std::vector&lt;Closedfish::Move&gt; representation.

Usage:

- `./pgn-extract.exe -Wlalg <path>/<to>/<filename>.pgn > <path>/<to>/parsed.txt`
- `g++ -std=c++17 -O2 reader.cpp -o reader` and `./reader <path>/<to>/parsed.txt`

Example data comes from https://www.chessgames.com/perl/chessgame?gid=1073545&m=24
