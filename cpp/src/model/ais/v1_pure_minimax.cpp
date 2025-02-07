#include "../pieces/piece.hpp"
#include "../utils/threadpool.hpp"
#include "ai.hpp"

#include <map>

#define MULTITHREADED 1

static int INFINITY = std::numeric_limits<int>::max();

int position_counter;

Move ai::v1_pure_minimax::_search(const Board& b) {
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
    std::cout << "Looked at " << position_counter << " positions" << std::endl;
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
            case Piece::King:
            case Piece::None:
                break;
            }
    }
    return ret;
}

int ai::v1_pure_minimax::eval(const Board& b) {
    position_counter++;
    int white_eval = count_material(b, Colour::White);
    int black_eval = count_material(b, Colour::Black);

    int evaluation = white_eval - black_eval;

    int perspective = b.white_to_play ? 1 : -1;

    return perspective * evaluation;
}
