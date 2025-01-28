from .piece import Piece

class Pawn(Piece):
    def __init__(self, pos) -> None:
        super().__init__(pos)
        self.already_moved = False

