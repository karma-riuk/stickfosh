from logic.move import Move
from .piece import Piece

class Bishop(Piece):
    def legal_moves(self, board: "Board") -> list[Move]:
        ret = []

        # looking north east
        ret.extend(self._look_direction(board, 1, 1))

        # looking south east
        ret.extend(self._look_direction(board, 1, -1))

        # looking south west
        ret.extend(self._look_direction(board, -1, -1))

        # looking north west
        ret.extend(self._look_direction(board, -1, 1))

        return ret

