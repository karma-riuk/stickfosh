#pragma once

#include "coords.hpp"
#include "move.hpp"
#include "pieces/piece.hpp"

#include <string>

struct Board {
  private:
    int8_t get_king_of(int8_t) const;
    bool no_legal_moves_for(int8_t) const;

  public:
    int8_t squares[64] = {Piece::None};
    bool white_to_play = true;
    int8_t w_castle_rights = CastleSide::NeitherSide;
    int8_t b_castle_rights = CastleSide::NeitherSide;
    int8_t en_passant_target = -1;
    uint8_t n_half_moves = 0;
    uint8_t n_full_moves = 0;

    static Board setup_fen_position(std::string fen);

    Board make_move(Move) const;
    std::string to_fen() const;
    bool is_check_for(int8_t) const;

    std::vector<Move> all_legal_moves() const;

    bool is_checkmate_for(int8_t colour) const {
        return is_checkmate_for(colour) && no_legal_moves_for(colour);
    }

    bool is_stalemate_for(int8_t colour) const {
        return !is_checkmate_for(colour) && no_legal_moves_for(colour);
    }

    bool is_terminal() const {
        return is_checkmate_for(White) || is_checkmate_for(Black)
            || is_stalemate_for(White) || is_stalemate_for(Black);
    }

    int8_t colour_at(int8_t idx) const {
        return squares[idx] & 0b11000;
    }

    int8_t colour_at(Coords xy) const {
        return colour_at(xy.to_index());
    }
};
