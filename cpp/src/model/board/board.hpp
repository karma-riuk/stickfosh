#pragma once

#include "../pieces/piece.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"

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
    bool insufficient_material_for(Colour) const;

    bool insufficient_material() const {
        return insufficient_material_for(White)
            && insufficient_material_for(Black);
    };

    std::vector<Move> all_legal_moves() const;

    bool is_checkmate_for(int8_t colour) const {
        return is_check_for(colour) && no_legal_moves_for(colour);
    }

    bool is_stalemate_for(int8_t colour) const {
        return !is_check_for(colour) && no_legal_moves_for(colour);
    }

    bool is_terminal() const {
        return insufficient_material() || white_to_play
                 ? is_checkmate_for(White) || is_stalemate_for(White)
                 : is_checkmate_for(Black) || is_stalemate_for(Black);
    }

    Piece piece_at(int8_t idx) const {
        return (Piece) (squares[idx] & 0b00111);
    }

    Piece piece_at(Coords xy) const {
        return piece_at(xy.to_index());
    }

    Colour colour_at(int8_t idx) const {
        return (Colour) (squares[idx] & 0b11000);
    }

    Colour colour_at(Coords xy) const {
        return colour_at(xy.to_index());
    }
};

inline bool operator<(const Board& m1, const Board& m2) {
    return m1.to_fen() < m2.to_fen(
           ); // TODO: make this the comparison between the hash of the board
}
