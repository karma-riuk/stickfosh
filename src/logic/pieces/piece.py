from logic.move import Move, PieceMove
from logic.position import Position
from enum import Enum


class Colour(Enum):
    WHITE = "white"
    BLACK = "black"

class Piece:
    def __init__(self, pos: Position, colour: Colour) -> None:
        self.pos = pos
        assert colour == Colour.WHITE or colour == Colour.BLACK, "The colour of the piece must be either Piece.WHITE or Piece.BLACK"
        self.colour = colour

    def _move_for_position(self, board: "Board", x: int, y: int) -> Move | None:
        if not Position.is_within_bounds(x, y):
            return None
        piece = board.piece_at(x, y) 

        if piece is None:
            return PieceMove(self, Position(x, y))

        if piece.colour != self.colour:
            return PieceMove(self, Position(x, y), is_capturing=True)
        return None

    def position(self) -> Position:
        return self.pos

    def legal_moves(self, board: "Board") -> list["Move"]:
        raise NotImplementedError(f"Can't say what the legal moves are for {type(self).__name__}, the method hasn't been implemented yet")
