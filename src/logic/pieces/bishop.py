from logic.move import Move, PieceMove
from logic.position import Position
from .piece import Piece

class Bishop(Piece):
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

