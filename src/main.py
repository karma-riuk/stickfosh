from logic.board import create_board
from view.tui import TUI

if __name__ == "__main__":
    board = create_board()

    view = TUI(board)

    view.show()
