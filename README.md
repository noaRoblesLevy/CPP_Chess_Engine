# CPP Chess Engine

A chess engine written from scratch in C++, as a personal project to really understand the language and to take a deep dive into how chess engines work. Every part is built step by step, without external chess libraries.

## Current status

Milestone 6 of 10 is done. The engine can:

- load a position from the board part of a [FEN](https://www.chessprogramming.org/Forsyth-Edwards_Notation) string and print the board
- generate all pseudo-legal moves for every piece (sliding pieces, knights, king, pawns)
- handle the special moves: promotion (to queen, rook, bishop or knight), en passant and castling, including castling rights
- make and unmake any move exactly, also several moves deep, so the board and the castling rights are restored
- detect attacked squares and check, and keep only legal moves (no castling out of, through or into check)
- count move trees with perft

Verified with [perft](https://www.chessprogramming.org/Perft_Results): the engine matches the official node counts, for example 4 865 609 at depth 5 from the start position and 4 085 603 at depth 4 in the Kiwipete test position.

## Roadmap

| # | Milestone | Status |
|---|---|---|
| 1 | Print the board (8x8 array) | done |
| 2 | Load a position from FEN | done |
| 3 | Moves of a single piece (knight, king) | done |
| 4 | All pseudo-legal moves (sliding pieces, pawns) | done |
| 5 | Make and unmake moves, promotion, en passant, castling | done |
| 6 | Legal moves (check detection) and perft testing | done |
| 7 | UCI protocol, to play against it in a chess GUI | next |
| 8 | Evaluation (material, piece-square tables) | |
| 9 | Search (minimax, alpha-beta) | |
| 10 | Speed and strength (move ordering, quiescence search, transposition table, bitboards) | |

## Design

- **Board**: `char board[8][8]`, uppercase letters for white, lowercase for black, `.` for an empty square. Index `[0]` is rank 8.
- **Moves**: a `struct Move` with the from and to squares, plus what is needed to undo it: the captured piece, the promotion piece, flags for en passant and castling, and the castling rights from before the move.
- **Make / unmake**: played moves are kept on a stack (`std::vector<Move>`), so the last move can always be undone.

## Building

With g++ (C++20):

```
g++ -std=c++20 -Wall -Wextra -O2 src/main.cpp -o engine
./engine
```

Or with `make` on Linux / WSL. On Windows the project is set up for VS Code with MSYS2 MinGW: `Ctrl+Shift+B` builds, `F5` starts the debugger (gdb).

## Resources

- [Chess Programming Wiki](https://www.chessprogramming.org)
