from logic.position import Position


class Piece:
    WHITE = "white"
    BLACK = "black"

    def __init__(self, pos, colour) -> None:
        self.pos = pos
        assert colour == self.WHITE or colour == self.BLACK, "The colour of the piece must be either Piece.WHITE or Piece.BLACK"
        self.colour = colour

    def position(self) -> Position:
        return self.pos

    def legal_moves(self, board) -> list[Position]:
        raise NotImplementedError(f"Can't say what the legal moves are for {type(self).__name__}, the method hasn't been implemented yet")
