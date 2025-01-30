from logic.position import Position
from logic.move import Move
from enum import Enum


class Colour(Enum):
    WHITE = "white"
    BLACK = "black"

class Piece:

    def __init__(self, pos: Position, colour: Colour) -> None:
        self.pos = pos
        assert colour == Colour.WHITE or colour == Colour.BLACK, "The colour of the piece must be either Piece.WHITE or Piece.BLACK"
        self.colour = colour

    def position(self) -> Position:
        return self.pos

    def legal_moves(self, board) -> list[Move]:
        raise NotImplementedError(f"Can't say what the legal moves are for {type(self).__name__}, the method hasn't been implemented yet")
