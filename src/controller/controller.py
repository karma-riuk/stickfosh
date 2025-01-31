from logic.board import Board
from view.view import View


class Controller:
    def __init__(self, board: Board, view: View) -> None:
        self._board = board
        self._view = view

        self._view.set_controller(self)

    def on_tile_selected(self, x: int, y: int) -> None:
        raise NotImplementedError(f"Cannot handle tile selected event, {type(self).__name__} did not implement it")
