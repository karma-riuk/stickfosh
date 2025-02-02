#include "board.hpp"

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
    for (int rank = 7; rank >= 0; rank--) {
        int empty_cell_counter = 0;
        for (int file = 0; file < 8; file++) {
            if (this->squares[rank * 8 + file] == Piece::None) {
                empty_cell_counter++;
                continue;
            }

            int full_piece = this->squares[rank * 8 + file];
            char piece = p2c[full_piece & 0b111];
            Colour colour = (full_piece & 0b11000) == Colour::White
                              ? Colour::White
                              : Colour::Black;

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
    return ret;
}

Board Board::make_move(Move move) const {
    int8_t dest_piece = this->squares[move.target_square];

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
