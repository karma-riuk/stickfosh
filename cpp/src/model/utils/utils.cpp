#include "utils.hpp"

std::vector<int8_t> to_target_square(std::vector<Move> moves) {
    std::vector<int8_t> ret;
    for (Move move : moves)
        ret.push_back(move.target_square);
    return ret;
}
