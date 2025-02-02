#include "../board.hpp"
#include "../coords.hpp"
#include "../move.hpp"
#include "piece.hpp"

#include <vector>

std::vector<Move> rook_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret;
    auto e = look_direction(b, xy, 1, 0);
    ret.insert(ret.end(), e.begin(), e.end());

    auto s = look_direction(b, xy, 0, -1);
    ret.insert(ret.end(), s.begin(), s.end());

    auto w = look_direction(b, xy, -1, 0);
    ret.insert(ret.end(), w.begin(), w.end());

    auto n = look_direction(b, xy, 0, 1);
    ret.insert(ret.end(), n.begin(), n.end());

    return ret;
}
