from logic.pieces.piece import Piece
from logic.position import Position
from enum import Enum

class Move:
    def to_algebraic(self) -> str:
        raise NotImplementedError("The move can't be translated to algbraic notation, as it was not implemented")

    @staticmethod
    def from_algebraic(move: str) -> "Move":
        raise NotImplementedError("The move can't be translated from algbraic notation, as it was not implemented")


class PieceMove(Move):
    def __init__(self, piece: Piece, pos: Position,/, is_capturing: bool = False) -> None:
        super().__init__()
        self.piece = piece
        self.pos = pos
        self.is_capturing = is_capturing

class Castle(Move, Enum):
    KING_SIDE_CASTLE = "O-O"
    QUEEN_SIDE_CASTLE = "O-O-O"
