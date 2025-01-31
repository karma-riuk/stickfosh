from logic.board import Board
from view.view import View
from .controller import Controller


class GuiController(Controller):
    def __init__(self, board: Board, view: View) -> None:
        super().__init__(board, view)
        self._view.update_board(self._board, None, [])

    def on_tile_selected(self, x: int, y: int) -> None:
        piece = self._board.piece_at(x, y)
        print(f"Clicked on {x, y}, {piece = }")

        if piece:
            self._view.update_board(self._board, piece, piece.legal_moves(self._board))
        else:
            self._view.update_board(self._board, None, [])


