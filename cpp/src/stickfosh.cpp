#include "stickfosh.hpp"

#include "board.hpp"
#include "move.hpp"

#include <chrono>
#include <map>
#include <ostream>
#include <sstream>
#include <string>
#include <vector>

static std::string tick = "\033[92m✔️\033[0m";   // Green Tick;
static std::string cross = "\033[91m❌\033[0m"; // Red Cross

static std::map<std::string, std::map<int, int>> pos2expected{
    // -- Position 1
    {
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",
        {
            {1, 20},
            {2, 400},
            {3, 8902},
            {4, 197281},
        },
    },

    // -- Position 2
    {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1",
        {
            {1, 48},
            {2, 2039},
            {3, 97862},
            {4, 4085603},
        },
    },
};

static std::stringstream res;

int move_generation_test(Board& b, int depth, int max_depth) {
    if (depth == max_depth) {
        res.str("");
        res.clear();
    }

    if (b.is_terminal())
        return 0;
    if (depth == 0)
        return 1;

    std::vector<Move> moves = b.all_legal_moves();
    if (depth == 1)
        return moves.size();

    int num_pos = 0;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        // std::cout << ">" << move << std::endl;
        int n = move_generation_test(tmp_board, depth - 1, max_depth);
        // std::cout << "<" << move << std::endl;
        if (depth == max_depth)
            res << move << ": " << n << std::endl;
        num_pos += n;
    }
    return num_pos;
}

void perft() {
    for (auto& [key, value] : pos2expected)
        perft(key);
}

void perft(std::string pos) {
    std::cout << pos << std::endl;
    std::map<int, int> expected = pos2expected[pos];
    Board b = Board::setup_fen_position(pos);

    for (const auto& [depth, expected_n_moves] : expected) {
        std::cout << "Depth: " << depth << " " << std::flush;
        auto start = std::chrono::steady_clock::now();
        int moves = move_generation_test(b, depth, depth);
        auto end = std::chrono::steady_clock::now();
        auto elapsed =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();

        std::cout << "Results: " << moves << " ";
        if (moves == expected_n_moves)
            std::cout << tick << " ";
        else
            std::cout << cross << " (expected " << expected_n_moves << ") ";
        std::cout << "positions Time: " << elapsed << " milliseconds"
                  << std::endl;
        if (moves != expected_n_moves) {
            std::cout << std::endl << res.str();
            exit(1);
        }
    }
}
