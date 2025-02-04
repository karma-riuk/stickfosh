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
            {1, 20},     // 0
            {2, 400},    // 1
            {3, 8902},   // 38
            {4, 197281}, // 971
            // {5, 4865609}, // 23032
        },
    },

    // -- Position 2
    {
        "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 2",
        {
            {1, 48},    // 0
            {2, 2039},  // 16
            {3, 97862}, // 602
            // {4, 4085603}, // 26612
        },
    },

    // -- Position 3
    {
        "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 3",
        {
            {1, 14},     // 0
            {2, 191},    // 1
            {3, 2812},   // 11
            {4, 43238},  // 157
            {5, 674624}, // 2199
            // {6, 11030083},
        },
    },

    // -- Position 4a
    {
        "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 4",
        {
            {1, 6},      // 0
            {2, 264},    // 1
            {3, 9467},   // 69
            {4, 422333}, // 3085
            // {5, 15833292}, // 124452
        },
    },

    // -- Position 4b
    {
        "r2q1rk1/pP1p2pp/Q4n2/bbp1p3/Np6/1B3NBn/pPPP1PPP/R3K2R b KQ - 0 5",
        {
            {1, 6},      // 0
            {2, 264},    // 2
            {3, 9467},   // 104
            {4, 422333}, // 3742
            // {5, 15833292}, // 136784
        },
    },

    // -- Position 5
    {
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 6",
        {
            {1, 44},    // 0
            {2, 1486},  // 12
            {3, 62379}, // 357
            // {4, 2103487}, // 13804
            // {5, 89941194}, // 1230428
        },
    },

    // -- Position 6
    {
        "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 "
        "7",
        {
            {1, 46},    // 0
            {2, 2079},  // 16
            {3, 89890}, // 602
            // {4, 3894594}, // 26612
            // {5, 164075551}, // 1230428
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
