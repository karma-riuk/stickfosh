#include "../pieces/piece.hpp"
#include "../utils/threadpool.hpp"
#include "../utils/utils.hpp"
#include "ai.hpp"

#include <map>

#define MULTITHREADED 1


static int position_counter;

Move ai::v2_alpha_beta::_search(const Board& b) {
    position_counter = 0;
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
                            return _search(tmp_board, 3, -INFINITY, INFINITY);
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
    std::cout << "Looked at " << position_counter << " positions" << std::endl;
    return best_move;
}

int ai::v2_alpha_beta::_search(const Board& b, int depth, int alpha, int beta) {
    if (depth == 0 || stop_computation)
        return eval(b);

    if (b.no_legal_moves()) {
        if (b.is_check())
            return -INFINITY;
        return 0;
    }

    std::vector<Move> moves = b.all_legal_moves();

    Move best_move;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -_search(tmp_board, depth - 1, -beta, -alpha);
        if (tmp_eval >= beta)
            return beta;
        alpha = std::max(alpha, tmp_eval);
    }
    return alpha;
}

int ai::v2_alpha_beta::eval(const Board& b) {
    position_counter++;
    int white_eval = count_material(b, Colour::White);
    int black_eval = count_material(b, Colour::Black);

    int evaluation = white_eval - black_eval;

    int perspective = b.white_to_play ? 1 : -1;

    return perspective * evaluation;
}
