from logic.move import Move
from .piece import Piece

class Bishop(Piece):
    def legal_moves(self, board: "Board", / , looking_for_check = False) -> list[Move]:
        ret = []

        # looking north east
        ret.extend(self._look_direction(board, 1, 1))

        # looking south east
        ret.extend(self._look_direction(board, 1, -1))

        # looking south west
        ret.extend(self._look_direction(board, -1, -1))

        # looking north west
        ret.extend(self._look_direction(board, -1, 1))

        if not looking_for_check and board.is_check_for(self.colour):
            return self.keep_only_blocking(ret, board)

        return ret

