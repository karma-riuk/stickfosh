#include "ai.hpp"

std::string ai::v1_simple::search(std::string pos, int depth) {
    Board b = Board::setup_fen_position(pos);
    std::vector<Move> moves = b.all_legal_moves();

    return moves[rand() % moves.size()].to_string();
}
