from .piece import Piece

class King(Piece):
    def legal_moves(self, board: "Board") -> list["Move"]:
        ret = []
        for dx in [-1, 0, 1]:
            for dy in [-1, 0, 1]:
                if dx == 0 and dy == 0: # skip current position
                    continue
                x = self.pos.x + dx
                y = self.pos.y + dy
                move = self._move_for_position(board, x, y)
                if move:
                    board_after_move = board.make_move(move)
                    if not board_after_move.is_check_for(self.colour):
                        ret.append(move)

        return ret


