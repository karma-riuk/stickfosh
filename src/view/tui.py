from logic.board import Board
from logic.pieces.bishop import Bishop
from logic.pieces.king import King
from logic.pieces.knight import Knight
from logic.pieces.pawn import Pawn
from logic.pieces.piece import Piece
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
from view.view import View

class TUI(View):
    def __init__(self, board: Board) -> None:
        super().__init__(board)

    def show(self) -> None:
        board_view = [
            [" " for _ in range(0, 8)]
            for _ in range(0, 8)
        ]

        for pos, piece in self.board._white.items():
            board_view[pos.y][pos.x] = self.string_of(piece).upper()

        for pos, piece in self.board._black.items():
            board_view[pos.y][pos.x] = self.string_of(piece)

        # we reverse the board because (0, 0) in in the bottom left, not top left
        board_view.reverse()
        print(self.to_string(board_view))

    def to_string(self, board_view: list[list[str]]) -> str:
        VER_SEP = "|"
        HOR_SEP = "-"
        ROW_SEP = HOR_SEP * (2*len(board_view[0]) + 1)
        ret = ROW_SEP + "\n"
        for row_view in board_view:
            row_str = VER_SEP + VER_SEP.join(row_view) + VER_SEP
            ret += row_str + "\n"
            ret += ROW_SEP + "\n"

        return ret

    def string_of(self, piece: Piece) -> str:
        type_ = type(piece) 
        if type_ == Pawn:
            return "p"
        if type_ == Queen:
            return "q"
        if type_ == Bishop:
            return "b"
        if type_ == Knight:
            return "n"
        if type_ == Rook:
            return "r"
        if type_ == King:
            return "k"
        raise ValueError(f"Unknown piece type {type(piece)}")
