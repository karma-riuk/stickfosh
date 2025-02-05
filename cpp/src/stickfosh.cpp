#include "stickfosh.hpp"

#include "pieces/piece.hpp"
#include "threadpool.hpp"

#include <future>
#include <map>

static int INFINITY = std::numeric_limits<int>::max();

std::string search(std::string pos, int depth) {
    Board b = Board::setup_fen_position(pos);

    ThreadPool pool(std::thread::hardware_concurrency());

    std::vector<Move> moves = b.all_legal_moves();
    std::map<std::string, std::future<int>> futures;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        futures.insert(
            {move.to_string(), pool.enqueue(minimax, tmp_board, depth - 1)}
        );
    }

    std::string best_move;
    int best_eval = -INFINITY;
    for (auto& [move, future] : futures) {
        int eval = future.get();
        if (eval > best_eval) {
            best_eval = eval;
            best_move = move;
        }
    }

    return best_move;
}

int minimax(const Board& b, int depth) {
    if (b.is_checkmate_for(b.white_to_play ? White : Black))
        return -INFINITY;

    if (depth == 0)
        return eval(b);

    std::vector<Move> moves = b.all_legal_moves();
    int best_evaluation = 0;
    Move best_move;
    for (const Move& move : moves) {
        Board tmp_board = b.make_move(move);
        int tmp_eval = -minimax(tmp_board, depth - 1);
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
            switch (b.squares[i] & 0b111) {
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

int eval(const Board& b) {
    int white_eval = count_material(b, Colour::White);
    int black_eval = count_material(b, Colour::Black);

    int evaluation = white_eval - black_eval;

    int perspective = b.white_to_play ? 1 : -1;

    return perspective * evaluation;
}
