#include "../pieces/piece.hpp"
#include "../utils/utils.hpp"
#include "ai.hpp"

#include <algorithm>

static int force_king_to_corner(
    int8_t attacking_king, int8_t defending_king, float endgame_weight
) {
    int eval = 0;
    Coords def_xy = Coords::from_index(defending_king);

    Coords def_dist_to_center{
        std::max(3 - def_xy.x, def_xy.x - 4),
        std::max(3 - def_xy.y, def_xy.y - 4)
    };
    eval += def_dist_to_center.x + def_dist_to_center.y;

    // make attacking king go closer to defending king to cut off escape routes
    Coords attack_xy = Coords::from_index(attacking_king);
    Coords dist_between_kings{
        std::abs(attack_xy.x - def_xy.x),
        std::abs(attack_xy.y - def_xy.y)
    };
    int distance = dist_between_kings.x + dist_between_kings.y;
    eval += 14 - distance;

    return (int) (eval * 10 * endgame_weight);
}

static float endgame_phase_weight(int material_count_no_pawns) {
    static int endgame_material_start =
        RookValue * 2 + BishopValue + KnightValue;

    float multiplier = 1.f / endgame_material_start;
    return 1.f - std::min(1.f, material_count_no_pawns * multiplier);
}

int ai::v5_better_endgame::_eval(const Board& b) {
    int old_eval = v4_search_captures::_eval(b);
    Colour attacking_colour = b.white_to_play ? White : Black;
    Colour defending_colour = b.white_to_play ? Black : White;
    return old_eval
         + force_king_to_corner(
               b.get_king_of(attacking_colour),
               b.get_king_of(defending_colour),
               endgame_phase_weight(
                   count_material(b, attacking_colour, false)
                   + count_material(b, defending_colour, false)
               )
         );
}
