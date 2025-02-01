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
        self._white_castling_rights = set()
        self._black_castling_rights = set()
        self._en_passant_target = None
        self._n_moves = 0
        self._n_half_moves = 0

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
                if c == "-":
                    index += 1
                break

            sides = "kq"
            assert c in sides or c in sides.upper(), f"The FEN position is malformed, the castling rights should be either k or q (both either lower- or upper-case), instead is '{c}'"
            if c == "K":
                ret._white_castling_rights.add(CastleSide.King)
            if c == "Q":
                ret._white_castling_rights.add(CastleSide.Queen)
            if c == "k":
                ret._black_castling_rights.add(CastleSide.King)
            if c == "q":
                ret._black_castling_rights.add(CastleSide.Queen)

        # -- En passant target
        if position[index] != "-":
            pos = Position.from_algebraic(position[index:index+2]) 
            index += 2
            if pos.y == 2:
                pos.y += 1
                assert pos in ret._white, "En passant target is not in the position"
                ret._en_passant_target = ret._white[pos]
            elif pos.y == 5:
                pos.y -= 1
                assert pos in ret._black, "En passant target is not in the position"
                ret._en_passant_target = ret._black[pos]
            else:
                raise ValueError("You can't have a en passant target that is not on the third or sixth rank")
        else:
            index += 1
        index += 1

        ret._n_half_moves = int(position[index:position.find(" ", index + 1)])
        ret._n_moves = int(position[position.find(" ", index)+1:])

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

    def castling_rights_for(self, colour: Colour) -> set[CastleSide]:
        return self._white_castling_rights if colour == Colour.WHITE else self._black_castling_rights

    def make_move(self, move: Move) -> "Board":
        dest_piece = self.piece_at(move.pos.x, move.pos.y) 

        if dest_piece:
            assert dest_piece.colour != move.piece.colour, "A piece cannot cannot eat another piece of the same colour"

        # -- Copy current state
        ret = Board()
        ret._white = self._white.copy()
        ret._black = self._black.copy()
        ret._turn = Colour.WHITE if self._turn == Colour.BLACK else Colour.BLACK
        ret._white_castling_rights = self._white_castling_rights.copy()
        ret._black_castling_rights = self._black_castling_rights.copy()


        piece = move.piece

        # -- Actually make the move
        pieces_moving, other_pieces = (ret._white, ret._black) if piece.colour == Colour.WHITE else (ret._black, ret._white)

        del pieces_moving[piece.pos]
        pieces_moving[move.pos] = piece.move_to(move.pos)
        if move.pos in other_pieces:
            del other_pieces[move.pos]

        if piece.colour == Colour.BLACK:
            ret._n_moves = self._n_moves + 1

        if move.is_capturing or type(piece) == Pawn:
            ret._n_half_moves = 0
        else:
            ret._n_half_moves = self._n_half_moves + 1

        if move.en_passant:
            pos_to_remove = Position(move.pos.x, move.pos.y + (1 if self._turn == Colour.BLACK else -1))
            del other_pieces[pos_to_remove]

        if move.promotes_to is not None:
            assert type(piece) == Pawn, "Trying to promote something that is not a pawn: not good!"
            pieces_moving[move.pos] = move.promotes_to(move.pos, piece.colour)

        # -- Set en passant target if needed
        if move.becomes_en_passant_target:
            ret._en_passant_target = pieces_moving[move.pos]
        else:
            ret._en_passant_target = None

        # -- Handle castling (just move the rook over)
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

        # -- Check for castling rights
        if piece.colour == Colour.WHITE:
            if type(piece) == King:
                ret._white_castling_rights = set()

            if type(piece) == Rook:
                if piece.pos.x == 0 and CastleSide.Queen in ret._white_castling_rights:
                    ret._white_castling_rights.remove(CastleSide.Queen)
                elif piece.pos.x == 7 and CastleSide.King in ret._white_castling_rights:
                    ret._white_castling_rights.remove(CastleSide.King)
        else:
            if type(piece) == King:
                ret._black_castling_rights = set()

            if type(piece) == Rook:
                if piece.pos.x == 0 and CastleSide.Queen in ret._black_castling_rights:
                    ret._black_castling_rights.remove(CastleSide.Queen)
                elif piece.pos.x == 7 and CastleSide.King in ret._black_castling_rights:
                    ret._black_castling_rights.remove(CastleSide.King)


        return ret

    def to_fen_string(self):
        ret = ""
        for y in range(7, -1, -1):
            empty_cell_counter = 0
            for x in range(8):
                pos = Position(x, y)

                piece = None
                if pos in self._white:
                    piece = self._white[pos]
                elif pos in self._black:
                    piece = self._black[pos]

                if piece is None:
                    empty_cell_counter += 1
                    continue

                if empty_cell_counter > 0:
                    ret += str(empty_cell_counter)
                    empty_cell_counter = 0
                letter = piece.letter()
                ret += letter.lower() if piece.colour == Colour.BLACK else letter.upper()

            if empty_cell_counter > 0:
                ret += str(empty_cell_counter)

            if y > 0:
                ret += "/"
        ret += " "

        ret += "w" if self._turn == Colour.WHITE else "b"
        ret += " "

        if len(self._white_castling_rights) == 0 and len(self._black_castling_rights) == 0:
            ret += "-"
        else: 
            if CastleSide.King in self._white_castling_rights:
                ret += "K"
            if CastleSide.Queen in self._white_castling_rights:
                ret += "Q"

            if CastleSide.King in self._black_castling_rights:
                ret += "k"
            if CastleSide.Queen in self._black_castling_rights:
                ret += "q"
        ret += " "

        if self._en_passant_target is not None:
            pos = self._en_passant_target.pos
            pos.y += -1 if self._en_passant_target.colour == Colour.WHITE else 1
            ret += pos.to_algebraic()
        else:
            ret += "-"
        ret += " "

        ret += str(self._n_half_moves)
        ret += " "
        ret += str(self._n_moves)

        return ret

_fen_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
_fen_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
INITIAL_BOARD = Board.setup_FEN_position(_fen_pos)
