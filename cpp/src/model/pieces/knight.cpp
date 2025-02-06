#include "../board/board.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"
#include "piece.hpp"

#include <vector>

std::vector<Move> knight_moves(const Board& b, const Coords xy) {
    std::vector<Move> ret;
    std::vector<std::pair<int, int>> moves = {
        {+2, +1},
        {+1, +2}, // north east
        {+2, -1},
        {+1, -2}, // south east
        {-2, -1},
        {-1, -2}, // south west
        {-2, +1},
        {-1, +2} // north west
    };

    for (const auto& [dx, dy] : moves) {
        std::optional<Move> move =
            move_for_position(b, xy, Coords{xy.x + dx, xy.y + dy});
        if (move.has_value())
            ret.push_back(move.value());
    }
    return ret;
}
