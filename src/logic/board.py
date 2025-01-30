from logic.pieces.bishop import Bishop
from logic.pieces.king import King
from logic.pieces.knight import Knight
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
from logic.pieces.pawn import Pawn
from logic.pieces.piece import Piece
from logic.position import Position

from typing import Type

class Board:
    def __init__(self) -> None:
        self._white: dict[Position, Piece] = {}
        self._black: dict[Position, Piece] = {}
        self._turn = None
        self._white_castling_write = set()
        self._black_castling_write = set()
        self._en_passant_target = None

    @staticmethod
    def _piece_class_from_char(c: str) -> Type[Piece]:
        assert len(c) == 1, f"The piece {c} isn't denoted by 1 character"
        c = c.lower()
        if c == "p":
            return Pawn
        if c == "r":
            return Rook
        if c == "n":
            return Knight
        if c == "b":
            return Bishop
        if c == "q":
            return Queen
        if c == "k":
            return King
        raise ValueError(f"Unknown piece '{c}'")

    @staticmethod
    def setup_FEN_position(position: str) -> "Board":
        ret = Board()
        index = 0

        # -- Pieces
        pieces = "prnbqk" # possible pieces
        numbers = "12345678" # possible number of empty squares

        x = 0
        y = 7 # FEN starts from the top left, so 8th rank
        for c in position:
            index += 1
            if c == " ":
                break
            if c in pieces or c in pieces.upper():
                pos = Position(x, y)
                piece = Board._piece_class_from_char(c)
                if c.isupper():
                    ret._white[pos] = piece(pos, Piece.WHITE)
                else:
                    ret._black[pos] = piece(pos, Piece.BLACK)

                x += 1
                continue
            if c in numbers:
                x += int(c)
            if c == '/':
                x = 0
                y -= 1


        # -- Active colour
        if position[index] == "w":
            ret._turn = Piece.WHITE
        elif position[index] == "b":
            ret._turn = Piece.BLACK
        else:
            raise ValueError(f"The FEN position is malformed, the active colour should be either 'w' or 'b', but is '{position[index]}'")
        index += 1


        # -- Castling Rights
        for c in position[index:]:
            index += 1
            if c == "-" or c == " ":
                break

            sides = "kq"
            assert c in sides or c in sides.upper(), f"The FEN position is malformed, the castling rights should be either k or q (both either lower- or upper-case), instead is '{c}'"
            if c == "K":
                ret._white_castling_write.add(Board.KING_SIDE_CASTLE)
            if c == "Q":
                ret._white_castling_write.add(Board.QUEEN_SIDE_CASTLE)
            if c == "k":
                ret._black_castling_write.add(Board.KING_SIDE_CASTLE)
            if c == "q":
                ret._black_castling_write.add(Board.QUEEN_SIDE_CASTLE)

        # -- En passant target
        if position[index] != "-":
            ret._en_passant_target = position[index:index+2]

        return ret

    def piece_at(self, x: int, y: int) -> Piece | None:
        pos = Position(x, y)
        white_piece = self._white.get(pos, None)
        black_piece = self._black.get(pos, None)

        assert white_piece == None or black_piece == None, f"There are two pieces at the same position {pos}, this shouldn't happen!"

        if white_piece != None:
            return white_piece
        return black_piece
