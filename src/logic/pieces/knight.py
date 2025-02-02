from .piece import Piece

class Knight(Piece):
    def letter(self):
        return "n"

    def legal_moves(self, board: "Board", / , looking_for_check = False) -> list["Move"]:
        ret = []
        for dx, dy in [
            (+2, +1), (+1, +2), # north east
            (+2, -1), (+1, -2), # south east
            (-2, -1), (-1, -2), # south west
            (-2, +1), (-1, +2), # north west
        ]:
            move = self._move_for_position(board, self.pos.x + dx, self.pos.y + dy)
            if move is not None:
                ret.append(move)

        if not looking_for_check:# and board.is_check_for(self.colour):
            return self.keep_only_blocking(ret, board)

        return ret

