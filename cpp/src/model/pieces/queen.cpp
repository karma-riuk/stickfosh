#include "../board/board.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"
#include "piece.hpp"

#include <vector>

std::vector<Move> queen_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret;
    auto e = look_direction(b, xy, 1, 0);
    ret.insert(ret.end(), e.begin(), e.end());

    auto s = look_direction(b, xy, 0, -1);
    ret.insert(ret.end(), s.begin(), s.end());

    auto w = look_direction(b, xy, -1, 0);
    ret.insert(ret.end(), w.begin(), w.end());

    auto n = look_direction(b, xy, 0, 1);
    ret.insert(ret.end(), n.begin(), n.end());

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
