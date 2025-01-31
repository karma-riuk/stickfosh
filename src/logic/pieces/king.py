from logic.move import CastleSide, Move
from logic.position import Position
from .piece import Piece

class King(Piece):
    def legal_moves(self, board: "Board") -> list[Move]:
        ret = []

        # -- Regular moves
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

        if board.is_check_for(self.colour):
            return self.keep_only_blocking(ret, board)

        # -- Castles
        castling_writes = board.castling_writes_for(self.colour)
        if len(castling_writes) == 0:
            return ret

        if CastleSide.King in castling_writes:
            clear = True
            for dx in range(1, 3):
                x = self.pos.x + dx
                y = self.pos.y
                if board.piece_at(x, y) is not None:
                    clear = False
                    break

                move = self._move_for_position(board, x, y)
                board_after_move = board.make_move(move)
                if board_after_move.is_check_for(self.colour):
                    clear = False
                    break

            if clear:
                ret.append(Move(self, Position(6, self.pos.y), castle_side=CastleSide.King))

        if CastleSide.Queen in castling_writes:
            clear = True
            for dx in range(1, 3):
                x = self.pos.x - dx
                y = self.pos.y

                if board.piece_at(x, y) is not None:
                    clear = False
                    break

                move = self._move_for_position(board, x, y)
                board_after_move = board.make_move(move)
                if board_after_move.is_check_for(self.colour):
                    clear = False
                    break

            if clear:
                ret.append(Move(self, Position(2, self.pos.y), castle_side=CastleSide.Queen))

        return ret


