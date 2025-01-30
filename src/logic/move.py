from logic.pieces.piece import Piece
from logic.position import Position
from enum import Enum

class Move:
    def __init__(self, is_capturing: bool) -> None:
        self.is_capturing = is_capturing

    def to_algebraic(self) -> str:
        raise NotImplementedError("The move can't be translated to algbraic notation, as it was not implemented")

    @staticmethod
    def from_algebraic(move: str) -> "Move":
        raise NotImplementedError("The move can't be translated from algbraic notation, as it was not implemented")


class PieceMove(Move):
    def __init__(self, piece: Piece, pos: Position,/, is_capturing: bool = False) -> None:
        super().__init__(is_capturing)
        self.piece = piece
        self.pos = pos

class Castle(Move, Enum):
    KING_SIDE_CASTLE = False
    QUEEN_SIDE_CASTLE = False
