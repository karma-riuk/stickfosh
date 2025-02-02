#include "../board.hpp"
#include "../coords.hpp"
#include "../move.hpp"
#include "piece.hpp"

#include <vector>

std::vector<Move> bishop_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret;
    auto ne = look_direction(b, xy, 1, 1);
    ret.insert(ret.end(), ne.begin(), ne.end());

    auto se = look_direction(b, xy, 1, -1);
    ret.insert(ret.end(), se.begin(), se.end());

    auto sw = look_direction(b, xy, -1, -1);
    ret.insert(ret.end(), sw.begin(), sw.end());

    auto nw = look_direction(b, xy, -1, 1);
    ret.insert(ret.end(), nw.begin(), nw.end());

    return ret;
}
