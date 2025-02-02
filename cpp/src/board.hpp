#pragma once

#include "coords.hpp"
#include "move.hpp"
#include "pieces/piece.hpp"

#include <string>

struct Board {
  private:
    int8_t get_king_of(int8_t colour) const;

  public:
    int8_t squares[64] = {Piece::None};
    bool white_to_play;
    CastleSide w_castle_rights;
    CastleSide b_castle_rights;

    static Board setup_fen_position(std::string fen);

    Board make_move(Move) const;
    std::string to_fen() const;
    bool is_check_for(int8_t) const;

    int8_t colour_at(int8_t idx) const {
        return squares[idx] & 0b11000;
    }

    int8_t colour_at(Coords xy) const {
        return colour_at(xy.to_index());
    }
};
