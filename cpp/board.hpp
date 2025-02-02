#pragma once

#include "piece.hpp"

#include <string>

enum CastleRights {
    KingSide = 1,
    QueenSide = 2,
};

class Board {
  private:
    int8_t squares[64] = {Piece::None};
    Colour turn;
    int8_t castle_rights;

  public:
    static Board* setup_fen_position(std::string fen);

    std::string to_fen();
};
