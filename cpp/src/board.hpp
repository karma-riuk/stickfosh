#pragma once

#include "pieces/piece.hpp"

#include <string>

class Board {
  private:
    int8_t squares[64] = {Piece::None};
    Colour turn;
    int8_t w_castle_rights;
    int8_t b_castle_rights;

  public:
    static Board* setup_fen_position(std::string fen);

    std::string to_fen();
};
