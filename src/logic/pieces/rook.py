from logic.move import Move
from .piece import Piece

class Rook(Piece):
    def legal_moves(self, board: "Board") -> list[Move]:
        ret = []

        # looking east
        ret.extend(self._look_direction(board, 1, 0))

        # looking south
        ret.extend(self._look_direction(board, 0, -1))

        # looking west
        ret.extend(self._look_direction(board, -1, 0))

        # looking north
        ret.extend(self._look_direction(board, 0, 1))

        return ret
