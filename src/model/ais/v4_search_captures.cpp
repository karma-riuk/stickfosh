#include "../pieces/piece.hpp"
#include "../utils/utils.hpp"
#include "ai.hpp"

#include <algorithm>

#define MULTITHREADED 1

int ai::v4_search_captures::_ab_search(
    const Board& b, int depth, int alpha, int beta
) {
    if (depth == 0 || stop_computation)
        return _search_captures(b, alpha, beta);

    if (b.no_legal_moves()) {
        if (b.is_check())
            return -INFINITY;
        return 0;
    }

    std::vector<Move> moves = b.all_legal_moves();
    std::sort(moves.begin(), moves.end(), [&](Move& m1, Move& m2) {
        return m1.score_guess(b) > m2.score_guess(b);
    });

    Move best_move;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -_ab_search(tmp_board, depth - 1, -beta, -alpha);
        if (tmp_eval >= beta)
            return beta;
        alpha = std::max(alpha, tmp_eval);
    }
    return alpha;
}

int ai::v4_search_captures::_search_captures(
    const Board& b, int alpha, int beta
) {
    int evaluation = eval(b);
    if (evaluation >= beta)
        return beta;
    alpha = std::max(evaluation, alpha);

    std::vector<Move> moves = b.all_capturing_moves();
    std::sort(moves.begin(), moves.end(), [&](Move& m1, Move& m2) {
        return m1.score_guess(b) > m2.score_guess(b);
    });

    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -_search_captures(tmp_board, -beta, -alpha);
        if (tmp_eval >= beta)
            return beta;
        alpha = std::max(alpha, tmp_eval);
    }
    return alpha;
}
