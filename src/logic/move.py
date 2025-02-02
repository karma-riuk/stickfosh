# from logic.pieces.piece import Piece
from typing import Type
from logic.position import Position
from enum import Enum

class CastleSide(Enum):
    Neither = ""
    King = "O-O"
    Queen = "O-O-O"

class Move:
    def __init__(self, piece: "Piece", pos: Position,/, is_capturing: bool = False, castle_side: CastleSide = CastleSide.Neither, en_passant: bool = False, becomes_en_passant_target: bool = False, promotes_to: Type["Piece"] = None) -> None:
        self.piece = piece
        self.pos = pos
        self.is_capturing = is_capturing
        self.castle_side = castle_side
        self.becomes_en_passant_target = becomes_en_passant_target
        self.en_passant = en_passant
        self.promotes_to = promotes_to

    def to_algebraic(self) -> str:
        raise NotImplementedError("The move can't be translated to algbraic notation, as it was not implemented")

    @staticmethod
    def from_algebraic(move: str) -> "Move":
        raise NotImplementedError("The move can't be translated from algbraic notation, as it was not implemented")

    def __str__(self) -> str:
        if self.castle_side == CastleSide.King:
            return "O-O"
        if self.castle_side == CastleSide.Queen:
            return "O-O-O"

        ret = ""
        if type(self.piece).__name__ == "Pawn":
            if self.is_capturing:
                ret += self.piece.pos.to_algebraic()[0]
                ret += "x"
                ret += self.pos.to_algebraic()
            else:
                ret += self.pos.to_algebraic()
        else:
            ret += self.piece.letter().upper()
            if self.is_capturing:
                ret += "x"
            ret += str(self.pos)
        
        return ret
    
    def __repr__(self) -> str:
        return str(self)
