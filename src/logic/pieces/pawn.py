from logic.move import Move
from logic.pieces.piece import Colour, Piece
from logic.position import Position

class Pawn(Piece):
    def legal_moves(self, board, / , looking_for_check = False) -> list[Move]:
        ret = []

        # can we capture to the left?
        if self.pos.x > 0 and (
            (self.colour == Colour.WHITE and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y + 1)))
            or
            (self.colour == Colour.BLACK and (capturable_piece := board.piece_at(self.pos.x - 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(Move(self, capturable_piece.pos, is_capturing = True))

        # can we capture to the right?
        if self.pos.x < 7 and (
            (self.colour == Colour.WHITE and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y + 1)))
            or
            (self.colour == Colour.BLACK and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                ret.append(Move(self, capturable_piece.pos, is_capturing = True))

        if self.colour == Colour.WHITE:
            for dy in range(1, 3 if self.pos.y == 1 else 2):
                if self.pos.y + dy > 7 or board.piece_at(self.pos.x, self.pos.y + dy):
                    break
                pos = Position(self.pos.x, self.pos.y + dy)
                ret.append(Move(self, pos))
        else:
            for dy in range(1, 3 if self.pos.y == 6 else 2):
                if self.pos.y - dy < 0 or board.piece_at(self.pos.x, self.pos.y - dy):
                    break
                pos = Position(self.pos.x, self.pos.y - dy)
                ret.append(Move(self, pos))

        if not looking_for_check and board.is_check_for(self.colour):
            return self.keep_only_blocking(ret, board)

        return ret
