from logic.move import Move
from logic.pieces.bishop import Bishop
from logic.pieces.knight import Knight
from logic.pieces.piece import Colour, Piece
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
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
                if (self.colour == Colour.WHITE and capturable_piece.pos.y == 7) or (self.colour == Colour.BLACK and capturable_piece.pos.y == 0):
                    for piece in [Queen, Knight, Bishop, Rook]:
                        ret.append(Move(self, capturable_piece.pos, is_capturing=True, promotes_to=piece))
                else:
                    ret.append(Move(self, capturable_piece.pos, is_capturing = True))

        # can we capture to the right?
        if self.pos.x < 7 and (
            (self.colour == Colour.WHITE and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y + 1)))
            or
            (self.colour == Colour.BLACK and (capturable_piece := board.piece_at(self.pos.x + 1, self.pos.y - 1)))
        ):
            if capturable_piece.colour != self.colour:
                if (self.colour == Colour.WHITE and capturable_piece.pos.y == 7) or (self.colour == Colour.BLACK and capturable_piece.pos.y == 0):
                    for piece in [Queen, Knight, Bishop, Rook]:
                        ret.append(Move(self, capturable_piece.pos, is_capturing=True, promotes_to=piece))
                else:
                    ret.append(Move(self, capturable_piece.pos, is_capturing = True))

        # -- Can we capture en passant?
        if board._en_passant_target is not None and \
            board._en_passant_target.pos.y == self.pos.y and (
                board._en_passant_target.pos.x == self.pos.x - 1
                or board._en_passant_target.pos.x == self.pos.x + 1
        ):
            if board._en_passant_target.colour != self.colour:
                old_pos = board._en_passant_target.pos
                new_pos = Position(old_pos.x, old_pos.y + (1 if self.colour == Colour.WHITE else -1))
                ret.append(Move(self, new_pos, is_capturing = True, en_passant = True))

        # -- Normal moves
        if self.colour == Colour.WHITE:
            for dy in range(1, 3 if self.pos.y == 1 else 2):
                y = self.pos.y + dy
                if y > 7 or board.piece_at(self.pos.x, y):
                    break
                pos = Position(self.pos.x, y)
                if y == 7:
                    for piece in [Queen, Knight, Bishop, Rook]:
                        ret.append(Move(self, pos, promotes_to=piece))
                else:
                    ret.append(Move(self, pos, becomes_en_passant_target=dy==2))
        else:
            for dy in range(1, 3 if self.pos.y == 6 else 2):
                y = self.pos.y - dy
                if y < 0 or board.piece_at(self.pos.x, y):
                    break
                pos = Position(self.pos.x, y)
                if y == 0:
                    for piece in [Queen, Knight, Bishop, Rook]:
                        ret.append(Move(self, pos, promotes_to=piece))
                else:
                    ret.append(Move(self, pos, becomes_en_passant_target=dy==2))

        if not looking_for_check and board.is_check_for(self.colour):
            return self.keep_only_blocking(ret, board)

        return ret
