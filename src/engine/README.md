# SwitchEngine

Main chess engine, wrapping over both `ClosedfishEngine` and `StockfishEngine`.

### Usage

- The engine's constructor should be feed with `CFBoard` and `logger` references.
- `getNextMove` gives the next move. This corresponds to the `lib/Algo/Algorithm.h`'s `ChessEngine` specification.
