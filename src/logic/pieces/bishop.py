from logic.move import Move
from .piece import Piece

class Bishop(Piece):
    def legal_moves(self, board) -> list[Move]:
        return super().legal_moves(board)
