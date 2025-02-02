#pragma once

#include "pieces/piece.hpp"

#include <string>

struct Board {
  private:
    int8_t get_king_of(int8_t colour) const;

  public:
    int8_t squares[64] = {Piece::None};
    bool white_to_play;
    int8_t w_castle_rights;
    int8_t b_castle_rights;

    static Board setup_fen_position(std::string fen);

    Board make_move(Move) const;
    std::string to_fen() const;
    bool is_check_for(int8_t) const;
};
