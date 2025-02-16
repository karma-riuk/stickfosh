#include "../pieces/piece.hpp"
#include "../utils/threadpool.hpp"
#include "../utils/utils.hpp"
#include "ai.hpp"

#include <map>

#define MULTITHREADED 1

Move ai::v1_pure_minimax::_search(const Board& b) {
    std::vector<Move> moves = b.all_legal_moves();

    Move best_move;
    int best_eval = -INFINITY;
#if MULTITHREADED
    ThreadPool pool(std::thread::hardware_concurrency());

    std::cout << "Have to look at " << moves.size() << " moves" << std::endl;

    std::map<Move, std::future<int>> futures;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        futures.insert({move, pool.enqueue([&, tmp_board]() {
                            return _search(tmp_board, 3);
                        })});
    }

    int counter = 0;
    for (auto& [move, future] : futures) {
        int eval = future.get();
        counter++;
        if (!am_white)
            eval *= -1;
        if (eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }
#else
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        std::cout << "Looking at " << move << std::endl;
        int eval = _search(tmp_board, 3);
        if (!am_white)
            eval *= -1;
        if (eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }
#endif
    return best_move;
}

int ai::v1_pure_minimax::_search(const Board& b, int depth) {
    if (depth == 0 || stop_computation)
        return eval(b);

    if (b.no_legal_moves()) {
        if (b.is_check())
            return -INFINITY;
        return 0;
    }

    std::vector<Move> moves = b.all_legal_moves();

    int best_evaluation = -INFINITY;
    Move best_move;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -_search(tmp_board, depth - 1);
        best_evaluation = std::max(best_evaluation, tmp_eval);
    }
    return best_evaluation;
}

int ai::v1_pure_minimax::_eval(const Board& b) {
    int white_eval = count_material(b, Colour::White);
    int black_eval = count_material(b, Colour::Black);

    int evaluation = white_eval - black_eval;

    int perspective = b.white_to_play ? 1 : -1;

    return perspective * evaluation;
}
