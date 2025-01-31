from logic.move import Move
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

    def letter(self):
        return type(self).__name__[0].lower()

    def _look_direction(self, board: "Board", mult_dx: int, mult_dy: int):
        ret = []
        for d in range(1, 8):
            dx = mult_dx * d
            dy = mult_dy * d

            move = self._move_for_position(board, self.pos.x + dx, self.pos.y + dy)
            if move is None:
                break
            ret.append(move)
            if move.is_capturing:
                break

        return ret

    def _move_for_position(self, board: "Board", x: int, y: int) -> Move | None:
        if not Position.is_within_bounds(x, y):
            return None
        piece = board.piece_at(x, y) 

        if piece is None:
            return Move(self, Position(x, y))

        if piece.colour != self.colour:
            return Move(self, Position(x, y), is_capturing=True)
        return None

    def position(self) -> Position:
        return self.pos

    def move_to(self, pos: Position) -> "Piece":
        ret = type(self)(pos, self.colour)
        return ret

    def legal_moves(self, board: "Board") -> list["Move"]:
        raise NotImplementedError(f"Can't say what the legal moves are for {type(self).__name__}, the method hasn't been implemented yet")
