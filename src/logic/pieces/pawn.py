from logic.move import Move, PieceMove
from logic.pieces.piece import Piece
from logic.position import Position

class Pawn(Piece):
    def legal_moves(self, board) -> list[Move]:
        ret = []

        # can we capture to the left?
        if self.pos.x > 0 and (
            (self.colour == self.WHITE and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y + 1)))
            or
            (self.colour == self.BLACK and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(PieceMove(self, capturable_piece.pos, is_capturing = True))

        # can we capture to the right?
        if self.pos.x < 7 and (
            (self.colour == self.WHITE and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y + 1)))
            or
            (self.colour == self.BLACK and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(PieceMove(self, capturable_piece.pos, is_capturing = True))

        if self.colour == Piece.WHITE:
            for dy in range(1, 3 if self.pos.y == 1 else 2):
                if self.pos.y + dy > 7 or board.piece_at(self.pos.x, self.pos.y + dy):
                    break
                pos = Position(self.pos.x, self.pos.y + dy)
                ret.append(PieceMove(self, pos))
        else:
            for dy in range(1, 3 if self.pos.y == 6 else 2):
                if self.pos.y - dy < 0 or board.piece_at(self.pos.x, self.pos.y - dy):
                    break
                pos = Position(self.pos.x, self.pos.y - dy)
                ret.append(PieceMove(self, pos))


        print(ret)
        return ret
