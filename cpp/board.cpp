#include <cctype>
#include <map>
#include <string>

enum Piece {
    None = 0,
    King = 1,
    Pawn = 2,
    Knigt = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,
};

enum Colour {
    White = 8,
    Black = 16,
};

enum CastleRights {
    KingSide = 1,
    QueenSide = 2,
};

class Board {
  private:
    int squares[64] = {Piece::None};
    Colour turn;
    int castle_rights;

  public:
    static Board* setup_fen_position(std::string fen);

    std::string to_fen();
};

Board* Board::setup_fen_position(std::string fen) {
    Board* board = new Board();
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
            board->squares[rank * 8 + file] = colour | piece;
            file++;
        }
    }
    return board;
}

std::string Board::to_fen() {
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
