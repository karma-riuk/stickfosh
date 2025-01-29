from logic.position import Position
from logic.pieces.piece import Piece

class Pawn(Piece):
    def legal_moves(self, board) -> list[Position]:
        ret = []

        # can we capture to the left?
        if self.pos.x > 0 and (
            (self.colour == self.WHITE and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y + 1)))
            or
            (self.colour == self.BLACK and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(capturable_piece.pos)

        # can we capture to the right?
        if self.pos.x < 7 and (
            (self.colour == self.WHITE and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y + 1)))
            or
            (self.colour == self.BLACK and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(capturable_piece.pos)

        for dy in range(1, 3 if self.pos.y == 1 else 2):
            if self.pos.y + dy > 7 or board.piece_at(self.pos.x, self.pos.y + dy):
                break
            ret.append(Position(self.pos.x, self.pos.y + dy))

        return ret
