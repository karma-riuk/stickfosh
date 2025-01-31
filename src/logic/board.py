from logic.move import CastleSide, Move
from logic.pieces.bishop import Bishop
from logic.pieces.king import King
from logic.pieces.knight import Knight
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
from logic.pieces.pawn import Pawn
from logic.pieces.piece import Colour, Piece
from logic.position import Position

from typing import Type

class Board:
    def __init__(self) -> None:
        self._white: dict[Position, Piece] = {}
        self._black: dict[Position, Piece] = {}
        self._turn = None
        self._white_castling_write = set()
        self._black_castling_write = set()
        self._en_passant_target = None

    @staticmethod
    def _piece_class_from_char(c: str) -> Type[Piece]:
        assert len(c) == 1, f"The piece {c} isn't denoted by 1 character"
        c = c.lower()
        if c == "p":
            return Pawn
        if c == "r":
            return Rook
        if c == "n":
            return Knight
        if c == "b":
            return Bishop
        if c == "q":
            return Queen
        if c == "k":
            return King
        raise ValueError(f"Unknown piece '{c}'")

    @staticmethod
    def setup_FEN_position(position: str) -> "Board":
        ret = Board()
        index = 0

        # -- Pieces
        pieces = "prnbqk" # possible pieces
        numbers = "12345678" # possible number of empty squares

        x = 0
        y = 7 # FEN starts from the top left, so 8th rank
        for c in position:
            index += 1
            if c == " ":
                break
            if c in pieces or c in pieces.upper():
                pos = Position(x, y)
                piece = Board._piece_class_from_char(c)
                if c.isupper():
                    ret._white[pos] = piece(pos, Colour.WHITE)
                else:
                    ret._black[pos] = piece(pos, Colour.BLACK)

                x += 1
                continue
            if c in numbers:
                x += int(c)
            if c == '/':
                x = 0
                y -= 1


        # -- Active colour
        if position[index] == "w":
            ret._turn = Colour.WHITE
        elif position[index] == "b":
            ret._turn = Colour.BLACK
        else:
            raise ValueError(f"The FEN position is malformed, the active colour should be either 'w' or 'b', but is '{position[index]}'")
        index += 2


        # -- Castling Rights
        for c in position[index:]:
            index += 1
            if c == "-" or c == " ":
                break

            sides = "kq"
            assert c in sides or c in sides.upper(), f"The FEN position is malformed, the castling rights should be either k or q (both either lower- or upper-case), instead is '{c}'"
            if c == "K":
                ret._white_castling_write.add(CastleSide.King)
            if c == "Q":
                ret._white_castling_write.add(CastleSide.Queen)
            if c == "k":
                ret._black_castling_write.add(CastleSide.King)
            if c == "q":
                ret._black_castling_write.add(CastleSide.Queen)

        # -- En passant target
        if position[index] != "-":
            ret._en_passant_target = position[index:index+2]

        return ret

    def piece_at(self, x: int, y: int) -> Piece | None:
        pos = Position(x, y)
        white_piece = self._white.get(pos, None)
        black_piece = self._black.get(pos, None)

        assert white_piece == None or black_piece == None, f"There are two pieces at the same position {pos}, this shouldn't happen!"

        if white_piece != None:
            return white_piece
        return black_piece

    def is_check_for(self, colour: Colour) -> bool:
        """ Is it check for the defending colour passed as parameter """
        defending_pieces, attacking_pieces = (self._white, self._black) if colour == Colour.WHITE else (self._black, self._white)

        kings = [piece for piece in defending_pieces.values() if type(piece) == King]
        assert len(kings) == 1, f"We have more than one king for {colour}, that is no buono..."
        king = kings[0]

        for piece in attacking_pieces.values():
            possible_pos = []
            if type(piece) == King: 
                # special case for the king, because it creates infinite recursion (since he looks if he's walking into a check)
                for dx in [-1, 0, 1]:
                    for dy in [-1, 0, 1]:
                        x, y = piece.pos.x + dx, piece.pos.y + dy
                        if Position.is_within_bounds(x, y):
                            possible_pos.append(Position(x, y))
            else:
                possible_pos += [move.pos for move in piece.legal_moves(self, looking_for_check=True)]
            if king.pos in possible_pos:
                return True
        return False

    def is_checkmate_for(self, colour: Colour) -> bool:
        """ Is it checkmate for the defending colour passed as parameter """
        return self.is_check_for(colour) and self._no_legal_moves_for(colour)

    def is_stalemate_for(self, colour: Colour) -> bool:
        """ Is it stalemate for the defending colour passed as parameter """
        return not self.is_check_for(colour) and self._no_legal_moves_for(colour)

    def _no_legal_moves_for(self, colour: Colour) -> bool:
        """ Return true if there are indeed no legal moves for the given colour (for checkmate or stalemate)"""
        pieces = self._white if colour == Colour.WHITE else self._black
        for piece in pieces.values():
            if len(piece.legal_moves(self)) > 0:
                return False
        return True

    def castling_writes_for(self, colour: Colour) -> set[CastleSide]:
        return self._white_castling_write if colour == Colour.WHITE else self._black_castling_write

    def make_move(self, move: Move) -> "Board":
        dest_piece = self.piece_at(move.pos.x, move.pos.y) 

        if dest_piece:
            assert dest_piece.colour != move.piece.colour, "A piece cannot cannot eat another piece of the same colour"

        ret = Board()
        ret._white = self._white.copy()
        ret._black = self._black.copy()
        ret._turn = Colour.WHITE if self._turn == Colour.BLACK else Colour.BLACK
        ret._white_castling_write = self._white_castling_write.copy()
        ret._black_castling_write = self._black_castling_write.copy()
        ret._en_passant_target = self._en_passant_target

        piece = move.piece
        pieces_moving, other_pieces = (ret._white, ret._black) if piece.colour == Colour.WHITE else (ret._black, ret._white)

        del pieces_moving[piece.pos]
        pieces_moving[move.pos] = piece.move_to(move.pos)
        if move.pos in other_pieces:
            del other_pieces[move.pos]

        if move.castle_side == CastleSide.King:
            rook_pos = Position(7, piece.pos.y)
            assert rook_pos in pieces_moving and type(pieces_moving[rook_pos]) == Rook, "Either rook is absent from the king side or you are trying to castle with something else than a rook..."
            del pieces_moving[rook_pos]
            new_rook_pos = Position(5, piece.pos.y)
            pieces_moving[new_rook_pos] = Rook(new_rook_pos, piece.colour)

        elif move.castle_side == CastleSide.Queen:
            rook_pos = Position(0, piece.pos.y)
            assert rook_pos in pieces_moving and type(pieces_moving[rook_pos]) == Rook, "Either rook is absent from the queen side or you are trying to castle with something else than a rook..."
            del pieces_moving[rook_pos]
            new_rook_pos = Position(3, piece.pos.y)
            pieces_moving[new_rook_pos] = Rook(new_rook_pos, piece.colour)

        if piece.colour == Colour.WHITE:
            if type(piece) == King:
                ret._white_castling_write = set()

            if type(piece) == Rook:
                if piece.pos.x == 0 and CastleSide.Queen in ret._white_castling_write:
                    ret._white_castling_write.remove(CastleSide.Queen)
                elif piece.pos.x == 7 and CastleSide.King in ret._white_castling_write:
                    ret._white_castling_write.remove(CastleSide.King)
        else:
            if type(piece) == King:
                ret._black_castling_write = set()

            if type(piece) == Rook:
                if piece.pos.x == 0 and CastleSide.Queen in ret._black_castling_write:
                    ret._black_castling_write.remove(CastleSide.Queen)
                elif piece.pos.x == 7 and CastleSide.King in ret._black_castling_write:
                    ret._black_castling_write.remove(CastleSide.King)


        return ret
