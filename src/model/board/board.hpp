#pragma once

#include "../pieces/piece.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"

#include <string>

struct Board {
  private:
    bool _no_legal_moves_for(Colour) const;
    bool _is_check_for(Colour) const;
    bool nlm = false, check = false;

  public:
    int8_t squares[64] = {Piece::None};
    bool white_to_play = true;
    int8_t w_castle_rights = CastleSide::NeitherSide;
    int8_t b_castle_rights = CastleSide::NeitherSide;
    int8_t en_passant_target = -1;
    int n_half_moves = 0;
    int n_full_moves = 0;


    static Board setup_fen_position(std::string fen);

    int8_t get_king_of(int8_t) const;
    Board skip_turn() const;
    Board make_move(Move, bool = true) const;
    std::string to_fen() const;
    bool no_legal_moves() const;
    bool is_check() const;
    bool insufficient_material_for(Colour) const;

    bool insufficient_material() const {
        return insufficient_material_for(White)
            && insufficient_material_for(Black);
    };

    std::vector<Move> all_legal_moves() const;
    std::vector<Move> all_capturing_moves() const;
    std::vector<int8_t> opponent_pawn_attack_map() const;

    bool is_checkmate() const;

    bool is_stalemate() const;

    bool is_terminal() const;

    inline Piece piece_at(int8_t idx) const {
        return (Piece) (squares[idx] & 0b00111);
    }

    inline Piece piece_at(Coords xy) const {
        return piece_at(xy.to_index());
    }

    inline Colour colour_at(int8_t idx) const {
        return (Colour) (squares[idx] & 0b11000);
    }

    inline Colour colour_at(Coords xy) const {
        return colour_at(xy.to_index());
    }
};

inline bool operator<(const Board& m1, const Board& m2) {
    return m1.to_fen() < m2.to_fen(
           ); // TODO: make this the comparison between the hash of the board
}
