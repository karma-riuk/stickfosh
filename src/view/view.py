from logic.board import Board


class View:
    def __init__(self, board: Board) -> None:
        self.board: Board = board

    def show(self) -> None:
        raise NotImplementedError(f"Can't show the board, the show() method of {type(self)} is not implemented")

