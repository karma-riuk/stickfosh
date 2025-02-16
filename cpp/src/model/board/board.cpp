#include "board.hpp"

#include "../pieces/piece.hpp"
#include "../utils/coords.hpp"
#include "../utils/move.hpp"
#include "../utils/utils.hpp"

#include <SFML/Graphics/BlendMode.hpp>
#include <algorithm>
#include <cctype>
#include <map>
#include <sstream>
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

    board.check = board._is_check_for(board.white_to_play ? White : Black);
    board.nlm = board._no_legal_moves_for(board.white_to_play ? White : Black);

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
            if (piece_at({file, rank}) == Piece::None) {
                empty_cell_counter++;
                continue;
            }

            int full_piece = squares[rank * 8 + file];
            char piece = p2c[full_piece & 0b111];
            Colour colour = colour_at({file, rank});

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

Board Board::skip_turn() const {
    Board ret = *this;
    ret.white_to_play = !ret.white_to_play;
    ret.check = ret._is_check_for(ret.white_to_play ? White : Black);
    return ret;
}

Board Board::make_move(Move move, bool recurse_call) const {
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

    Piece source_piece = piece_at(move.source_square);
    Piece target_piece = piece_at(move.target_square);

    // -- Handle en passant target being eaten
    if (en_passant_target != -1 && source_piece == Piece::Pawn
        && target_piece == Piece::None)
        ret.squares[move.target_square + (white_to_play ? -8 : 8)] =
            Piece::None;

    // -- Handle promotion
    if (move.promoting_to != Piece::None)
        ret.squares[move.target_square] = move.promoting_to;

    // -- Set en passant target if need
    if (source_piece == Piece::Pawn
        && std::abs(move.target_square - move.source_square) == 16) {
        if (white_to_play)
            ret.en_passant_target = move.target_square - 8;
        else
            ret.en_passant_target = move.target_square + 8;
    } else {
        ret.en_passant_target = -1;
    }

    // -- Handle castling (just move the rook over)
    Coords c = Coords::from_index(move.source_square);
    if (source_piece == Piece::King) {
        if (move.target_square - move.source_square == 2) { // king side castle
            Coords rook_source{7, c.y};
            int8_t old_rook = ret.squares[rook_source.to_index()];
            ret.squares[rook_source.to_index()] = Piece::None;
            Coords rook_dest{5, c.y};
            ret.squares[rook_dest.to_index()] = old_rook;
        } else if (move.target_square - move.source_square == -2) { // queen
            Coords rook_source{0, c.y};
            int8_t old_rook = ret.squares[rook_source.to_index()];
            ret.squares[rook_source.to_index()] = Piece::None;
            Coords rook_dest{3, c.y};
            ret.squares[rook_dest.to_index()] = old_rook;
        }
    }

    // -- Check for castling rights
    ret.w_castle_rights = w_castle_rights;
    ret.b_castle_rights = b_castle_rights;
    bool is_capturing = squares[move.target_square] != Piece::None;
    if (white_to_play) {
        if (source_piece == King)
            ret.w_castle_rights = NeitherSide;

        if (source_piece == Rook) {
            if (c.x == 0 && (ret.w_castle_rights & QueenSide))
                ret.w_castle_rights &= ~(QueenSide);
            if (c.x == 7 && (ret.w_castle_rights & KingSide))
                ret.w_castle_rights &= ~(KingSide);
        }

        Coords target = Coords::from_index(move.target_square);
        if (is_capturing && target.y == 7 && target_piece == Rook) {
            if (target.x == 0 && (ret.b_castle_rights & QueenSide))
                ret.b_castle_rights &= ~(QueenSide);
            if (target.x == 7 && (ret.b_castle_rights & KingSide))
                ret.b_castle_rights &= ~(KingSide);
        }
    } else {
        if (source_piece == King)
            ret.b_castle_rights = NeitherSide;

        if (source_piece == Rook) {
            if (c.x == 0 && (ret.b_castle_rights & QueenSide))
                ret.b_castle_rights &= ~(QueenSide);
            if (c.x == 7 && (ret.b_castle_rights & KingSide))
                ret.b_castle_rights &= ~(KingSide);
        }

        Coords target = Coords::from_index(move.target_square);
        if (is_capturing && target.y == 0 && target_piece == Rook) {
            if (target.x == 0 && (ret.w_castle_rights & QueenSide))
                ret.w_castle_rights &= ~(QueenSide);
            if (target.x == 7 && (ret.w_castle_rights & KingSide))
                ret.w_castle_rights &= ~(KingSide);
        }
    }

    ret.n_half_moves = n_half_moves + 1;
    if (is_capturing || piece_at(move.source_square) == Piece::Pawn)
        ret.n_half_moves = 0;
    if (!white_to_play)
        ret.n_full_moves = n_full_moves + 1;

    if (ret.n_half_moves > 150) {
        std::cerr << "too many recursions" << std::endl;
        exit(1);
    }

    if (recurse_call) {
        ret.check = ret._is_check_for(ret.white_to_play ? White : Black);
        ret.nlm = ret._no_legal_moves_for(ret.white_to_play ? White : Black);
    }
    return ret;
}

bool Board::insufficient_material_for(Colour current_colour) const {
    int n_bishop = 0, n_knight = 0;

    for (int i = 0; i < 64; i++) {
        Colour colour = colour_at(i);
        if (colour != current_colour)
            continue;

        Piece piece = piece_at(i);
        if (piece == Piece::Pawn || piece == Piece::Queen
            || piece == Piece::Rook)
            return false;

        if (piece == Piece::Bishop)
            n_bishop++;
        if (piece == Piece::Knigt && colour == Colour::White)
            n_knight++;
    }
    return (n_bishop == 0 && n_knight == 0) || (n_bishop == 1 && n_knight == 0)
        || (n_bishop == 0 && n_knight == 1);
}

int8_t Board::get_king_of(int8_t colour) const {
    for (int i = 0; i < 64; i++)
        if (squares[i] == (colour | Piece::King))
            return i;
    std::stringstream ss;
    ss << "Apparently there no kings of the such color in this board: "
       << std::endl;
    ss << to_fen();
    throw std::domain_error(ss.str());
}

bool Board::_is_check_for(Colour colour) const {
    int8_t king_idx = this->get_king_of(colour);
    std::vector<Move> all_moves;
    all_moves.reserve(50);
    for (int8_t i = 0; i < 64; i++) {
        if (this->squares[i] == Piece::None || colour_at(i) == colour)
            continue;
        if (piece_at(i) == King) {
            // special case for the king, because it creates infinite recursion
            // (since he looks if he's walking into a check)
            Coords king_pos = Coords::from_index(i);
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    Coords c{king_pos.x + dx, king_pos.y + dy};
                    if (c.is_within_bounds())
                        all_moves.push_back(Move{i, c.to_index()});
                }
            }
        } else {
            std::vector<Move> moves = legal_moves(
                this->squares[i],
                *this,
                Coords::from_index(i),
                true
            );

            all_moves.insert(all_moves.end(), moves.begin(), moves.end());
        }

        for (const Move& move : all_moves)
            if (move.target_square == king_idx)
                return true;
        all_moves.clear();
    }
    return false;
}

bool Board::_no_legal_moves_for(Colour colour) const {
    for (int i = 0; i < 64; i++) {
        if (squares[i] == Piece::None || colour_at(i) != colour)
            continue;
        std::vector<Move> moves;
        moves = legal_moves(squares[i], *this, Coords::from_index(i));
        if (moves.size() > 0)
            return false;
    }
    return true;
}

bool Board::no_legal_moves() const {
    return nlm;
}

bool Board::is_check() const {
    return check;
}

bool Board::is_checkmate() const {
    return check && nlm;
}

bool Board::is_stalemate() const {
    return !check && nlm;
}

bool Board::is_terminal() const {
    return n_half_moves == 100 || insufficient_material() || is_checkmate()
        || is_stalemate();
}

std::vector<Move> Board::all_legal_moves() const {
    std::vector<Move> ret;
    for (int i = 0; i < 64; i++) {
        if ((colour_at(i) == White && white_to_play)
            || (colour_at(i) == Black && !white_to_play)) {
            std::vector<Move> moves =
                legal_moves(squares[i], *this, Coords::from_index(i));
            ret.insert(ret.end(), moves.begin(), moves.end());
        }
    }
    return ret;
}

std::vector<Move> Board::all_capturing_moves() const {
    std::vector<Move> moves = all_legal_moves();
    std::vector<Move> ret;
    ret.reserve(moves.size());
    for (const Move& move : moves)
        if (piece_at(move.target_square) != Piece::None)
            ret.push_back(move);

    return ret;
}

std::vector<int8_t> Board::opponent_pawn_attack_map() const {
    std::vector<int8_t> ret;
    for (int i = 0; i < 64; i++) {
        if (piece_at(i) == Piece::Pawn
            && ((colour_at(i) == White && !white_to_play)
                || (colour_at(i) == Black && white_to_play))) {
            std::vector<int8_t> attack_map =
                pawn_attack_map(*this, Coords::from_index(i));
            ret.insert(ret.end(), attack_map.begin(), attack_map.end());
        }
    }
    return ret;
}
