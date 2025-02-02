#include "board.hpp"

#include "coords.hpp"
#include "move.hpp"
#include "pieces/piece.hpp"

#include <algorithm>
#include <cctype>
#include <map>
#include <stdexcept>

Board Board::setup_fen_position(std::string fen) {
    Board board;
    std::map<char, Piece> c2p{
        {'k', Piece::King},
        {'p', Piece::Pawn},
        {'n', Piece::Knigt},
        {'b', Piece::Bishop},
        {'r', Piece::Rook},
        {'q', Piece::Queen},
    };

    // -- Pieces
    std::string fen_board = fen.substr(0, fen.find(' '));
    int rank = 7, file = 0;
    for (char symbol : fen_board) {
        if (symbol == '/') {
            file = 0;
            rank--;
            continue;
        }

        if (std::isdigit(symbol))
            file += symbol - '0';
        else {
            Colour colour =
                std::isupper(symbol) ? Colour::White : Colour::Black;

            Piece piece = c2p[std::tolower(symbol)];
            board.squares[rank * 8 + file] = colour | piece;
            file++;
        }
    }

    // -- Active colour
    int index = fen.find(' ');
    index++;
    board.white_to_play = fen[index] == 'w';

    // Castling Rights
    index += 2;
    for (char symbol : fen.substr(index, fen.find(' ', index + 1))) {
        index++;
        if (symbol == ' ' || symbol == '-') {
            if (symbol == '-')
                index++;
            break;
        }

        switch (symbol) {
        case 'K':
            board.w_castle_rights |= CastleSide::KingSide;
            break;
        case 'Q':
            board.w_castle_rights |= CastleSide::QueenSide;
            break;
        case 'k':
            board.b_castle_rights |= CastleSide::KingSide;
            break;
        case 'q':
            board.b_castle_rights |= CastleSide::QueenSide;
            break;
        }
    }

    // -- En passant target
    if (fen[index] != '-') {
        Coords c = Coords::from_algebraic(fen.substr(index, 2));
        index += 2;
        board.en_passant_target = c.to_index();
    } else {
        index++;
    }

    // -- Half move clock
    index = fen.find(' ', index) + 1;
    board.n_half_moves =
        std::stoi(fen.substr(index, fen.find(' ', index + 1) - index));

    // -- Full move number
    index = fen.find(' ', index) + 1;
    board.n_full_moves = std::stoi(fen.substr(index));


    return board;
}

std::string Board::to_fen() const {
    std::map<int, char> p2c{
        {Piece::King, 'k'},
        {Piece::Pawn, 'p'},
        {Piece::Knigt, 'n'},
        {Piece::Bishop, 'b'},
        {Piece::Rook, 'r'},
        {Piece::Queen, 'q'},
    };

    std::string ret;
    // -- Pieces
    for (int rank = 7; rank >= 0; rank--) {
        int empty_cell_counter = 0;
        for (int file = 0; file < 8; file++) {
            if (squares[rank * 8 + file] == Piece::None) {
                empty_cell_counter++;
                continue;
            }

            int full_piece = squares[rank * 8 + file];
            char piece = p2c[full_piece & 0b111];
            int8_t colour = colour_at({file, rank});

            if (empty_cell_counter > 0) {
                ret += std::to_string(empty_cell_counter);
                empty_cell_counter = 0;
            }
            ret += colour == Colour::White ? std::toupper(piece) : piece;
        }
        if (empty_cell_counter > 0)
            ret += std::to_string(empty_cell_counter);
        if (rank > 0)
            ret += "/";
    }
    ret += " ";

    // -- Active colour
    ret += white_to_play ? 'w' : 'b';
    ret += " ";

    // -- Castling Rights
    if (w_castle_rights == CastleSide::NeitherSide
        && b_castle_rights == CastleSide::NeitherSide)
        ret += '-';
    else {
        if (w_castle_rights & CastleSide::KingSide)
            ret += 'K';
        if (w_castle_rights & CastleSide::QueenSide)
            ret += 'Q';
        if (b_castle_rights & CastleSide::KingSide)
            ret += 'k';
        if (b_castle_rights & CastleSide::QueenSide)
            ret += 'q';
    }
    ret += ' ';

    // -- En passant target
    ret += en_passant_target == -1
             ? "-"
             : Coords::from_index(en_passant_target).to_algebraic();
    ret += ' ';

    // -- Half move clock
    ret += std::to_string(n_half_moves);
    ret += ' ';

    // -- Full moves number
    ret += std::to_string(n_full_moves);

    return ret;
}

Board Board::make_move(Move move) const {
    Board ret;
    std::copy(
        std::begin(this->squares),
        std::end(this->squares),
        std::begin(ret.squares)
    );
    ret.white_to_play = !this->white_to_play;

    // -- Actually make the move
    ret.squares[move.source_square] = Piece::None;
    ret.squares[move.target_square] = this->squares[move.source_square];

    // -- Handle en passant target being eaten
    if (move.en_passant)
        ret.squares[move.target_square - 8] = Piece::None;

    // -- Handle promotion
    if (move.promoting_to != 0)
        ret.squares[move.target_square] = move.promoting_to;

    // -- Set en passant target if need
    if ((squares[move.source_square] & 0b111) == Piece::Pawn
        && std::abs(move.target_square - move.source_square) == 16) {
        if (white_to_play)
            ret.en_passant_target = move.target_square - 8;
        else
            ret.en_passant_target = move.target_square + 8;
    } else {
        ret.en_passant_target = -1;
    }

    // -- Handle castling (just move the rook over)
    if (move.castle_side & KingSide) {
        Coords c = Coords::from_index(move.source_square);
        Coords rook_source{7, c.y};
        int8_t old_rook = ret.squares[rook_source.to_index()];
        ret.squares[rook_source.to_index()] = Piece::None;
        Coords rook_dest{5, c.y};
        ret.squares[rook_dest.to_index()] = old_rook;
    } else if (move.castle_side & QueenSide) {
        Coords c = Coords::from_index(move.source_square);
        Coords rook_source{0, c.y};
        int8_t old_rook = ret.squares[rook_source.to_index()];
        ret.squares[rook_source.to_index()] = Piece::None;
        Coords rook_dest{3, c.y};
        ret.squares[rook_dest.to_index()] = old_rook;
    }

    return ret;
}

int8_t Board::get_king_of(int8_t colour) const {
    for (int i = 0; i < 64; i++)
        if (this->squares[i] == (colour | Piece::King))
            return i;
    throw std::domain_error(
        "Apparently there no kings of the such color in this board"
    );
}

std::vector<int8_t> to_target_square(std::vector<Move> moves) {
    std::vector<int8_t> ret;
    for (Move move : moves)
        ret.push_back(move.target_square);
    return ret;
}

bool Board::is_check_for(int8_t colour) const {
    int8_t king_idx = this->get_king_of(colour);
    for (int i = 0; i < 64; i++) {
        if (this->squares[i] == Piece::None)
            continue;
        std::vector<Move> moves =
            legal_moves(this->squares[i], *this, Coords::from_index(i), true);
        std::vector<int8_t> targets = to_target_square(moves);
        if (std::find(targets.begin(), targets.end(), i) != targets.end())
            return true;
    }
    return false;
}
