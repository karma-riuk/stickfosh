from logic.board import Board
from logic.move import Move
from logic.pieces.piece import Piece


class View:
    def __init__(self) -> None:
        self._controller: "Controller" = None

    def show(self) -> None:
        raise NotImplementedError(f"Can't show the board, the show() method of {type(self)} is not implemented")

    def update_board(self, board: Board, selected_piece: Piece, legal_moves: list[Move]) -> None:
        raise NotImplementedError(f"Can't update the board, the update_board() method of {type(self)} is not implemented")

    def set_controller(self, controller: "Controller") -> None:
        self._controller = controller

