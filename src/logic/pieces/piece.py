from logic.position import Position


class Piece:
    def __init__(self, pos) -> None:
        self.pos = pos

    def position(self) -> Position:
        return self.pos

    def legal_moves(self, board) -> list[Position]:
        raise NotImplementedError(f"Can't say what the legal moves are for {type(self).__name__}, the method hasn't been implemented yet")
