# Stickfosh

> [Stockfish](https://stockfishchess.org), but worse :)

## Overview

This project is a **Chess AI** built using the **Model-View-Controller (MVC)
pattern**. It provides a modular framework for playing chess, allowing both
**human vs AI** and **AI vs AI** matches. The AI has undergone several
iterations, improving its decision-making capabilities through enhancements like
**alpha-beta pruning**, **move ordering**, **iterative deepening**, and
**transposition tables**.

## Features

- **MVC Architecture**: The project follows the MVC pattern for clean separation of concerns:
  - **Model**: Handles chess rules, board state, and AI logic.
  - **View**: GUI and NoOp (that shows nothing, but is useful for debugging the AIs) rendering options.
  - **Controller**: Manages interactions between players and the game.
- **Multiple AI Versions**: Several AI versions with increasing complexity have been implemented.
- **AI vs AI Matches**: A dedicated mode to watch different AI versions compete.
- **Human vs AI Mode**: Play against the AI using a graphical interface.
- **FEN Support**: Load chess positions using FEN notation.
- **Performance Testing (Perft)**: Built-in performance testing for move generation.

## AI Iterations

This project has undergone multiple AI improvements, including:

1. **v0 Random AI**: Selects moves randomly.
1. **v1 Pure Minimax**: Implements basic minimax search.
1. **v2 Alpha-Beta Pruning**: Optimizes minimax with pruning.
1. **v3 Move Ordering**: Prioritizes moves to improve search efficiency.
1. **v4 Search Captures**: Enhances move ordering by focusing on captures.
1. **v5 Better Endgame**: Introduces heuristics for endgame play.
1. **v6 Iterative Deepening**: Dynamically adjusts search depth for better performance.
1. **v7 Transposition Tables**: Caches board states to reduce redundant computations.

## Installation & Usage

### Prerequisites

- C++ Compiler (C++17 or later)
- `make`
- SFML (for GUI rendering)

### Build Instructions

1. Clone the repository:
   ```sh
   git clone https://github.com/karma-riuk/stickfosh.git
   cd stickfosh
   ```
1. Create a build directory and compile:
   ```sh
   make stickfosh
   ```
1. Run the program:
   ```sh
   ./stickfosh
   ```

## Running the Application

Stickfosh provides multiple execution modes, selectable via command-line arguments:

| Mode | Description | Example Command |
|------|------------|----------------|
| **Human vs AI** | Play against the AI | `./stickfosh --mode human_vs_ai` |
| **AI vs AI** | Watch two AI versions compete | `./stickfosh --mode ai_vs_ai --ai1 v3_AB_ordering --ai2 v6_iterative_deepening` |
| **Human vs Human** | Manually input moves for both sides | `./stickfosh --mode human_vs_human` |
| **Perft Testing** | Performance test move generation | `./stickfosh --mode perft` |
| **Custom FEN** | Start from a custom position | `./stickfosh --mode ai_vs_ai --fen "rnbqkb1r/pppppppp/8/8/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 1"` |

## Video Demo

![stickfosh.gif](./res/stickfosh.gif)


<!-- [![AI vs AI Chess Match](https://img.youtube.com/vi/XXXXXXXXXX/0.jpg)](https://www.youtube.com/watch?v=XXXXXXXXXX) -->

<!-- *Click the image above to watch a video of two AI versions competing!* -->

## Future Improvements

- Implement **opening book** for better early-game decisions.
- Enhance **evaluation function** with more advanced heuristics.
- Introduce **neural network-based AI** for machine-learning-driven play.
- Introduce **Monte-Carlo Tree Search** for stochastic driven play.
