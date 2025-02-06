#include "../pieces/piece.hpp"
#include "../utils/threadpool.hpp"
#include "ai.hpp"

static int INFINITY = std::numeric_limits<int>::max();

Move ai::v1_simple::_search(const Board& b) {
    ThreadPool pool(std::thread::hardware_concurrency());

    Move best_move;
    int best_eval = -INFINITY;
    std::vector<Move> moves = b.all_legal_moves();

    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int eval = _search(tmp_board, 4);
        if (!am_white)
            eval *= -1;
        if (eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }

    return best_move;
}

int ai::v1_simple::_search(const Board& b, int depth) {
    if (b.is_checkmate_for(b.white_to_play ? White : Black))
        return -INFINITY;

    if (depth == 0 || stop_computation)
        return eval(b);

    std::vector<Move> moves = b.all_legal_moves();
    int best_evaluation = 0;
    Move best_move;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -_search(tmp_board, depth - 1);
        best_evaluation = std::max(best_evaluation, tmp_eval);
    }
    return best_evaluation;
}

static int PawnValue = 100;
static int KnightValue = 300;
static int BishopValue = 320;
static int RookValue = 500;
static int QueenValue = 900;

int count_material(const Board& b, int8_t colour) {
    int ret = 0;
    for (int i = 0; i < 64; i++) {
        if (b.colour_at(i) == colour)
            switch (b.piece_at(i)) {
            case Piece::Pawn:
                ret += PawnValue;
                break;
            case Piece::Knigt:
                ret += KnightValue;
                break;
            case Piece::Bishop:
                ret += BishopValue;
                break;
            case Piece::Rook:
                ret += RookValue;
                break;
            case Piece::Queen:
                ret += QueenValue;
                break;
            }
    }
    return ret;
}

int ai::v1_simple::eval(const Board& b) {
    int white_eval = count_material(b, Colour::White);
    int black_eval = count_material(b, Colour::Black);

    int evaluation = white_eval - black_eval;

    int perspective = b.white_to_play ? 1 : -1;

    return perspective * evaluation;
}
