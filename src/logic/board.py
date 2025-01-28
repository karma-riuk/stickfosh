from logic.pieces.bishop import Bishop
from logic.pieces.king import King
from logic.pieces.knight import Knight
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
from logic.pieces.pawn import Pawn
from logic.pieces.piece import Piece
from logic.position import Position

class Board:
    def __init__(self) -> None:
        self._white: dict[Position, Piece] = {}
        self._black: dict[Position, Piece] = {}

        for x in range(8):
            pos_w_pawn = Position(x, 1)
            pos_b_pawn = Position(x, 6)

            self._white[pos_w_pawn] = Pawn(pos_w_pawn)
            self._black[pos_b_pawn] = Pawn(pos_b_pawn)

            pos_w_piece = Position(x, 0)
            pos_b_piece = Position(x, 7)

            piece = None
            if x == 0 or x == 7:
                piece = Rook
            elif x == 1 or x == 6:
                piece = Knight
            elif x == 2 or x == 5:
                piece = Bishop
            elif x == 3:
                piece = Queen
            elif x == 4:
                piece = King
            assert piece != None, f"Didn't know which piece to assign for {x = }"
            self._white[pos_w_piece] = piece(pos_w_piece)
            self._black[pos_b_piece] = piece(pos_b_piece)


def create_board():
    return Board()
