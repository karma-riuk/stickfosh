from logic.board import Board
from view.gui import GUI
from view.tui import TUI

if __name__ == "__main__":
    initial_board_position = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
    board = Board.setup_FEN_position(initial_board_position)

    view = GUI(board)

    view.show()
