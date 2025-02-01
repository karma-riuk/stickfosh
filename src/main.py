from controller.controller import Controller
from logic.board import INITIAL_BOARD
from view.gui import GUI
from view.tui import TUI

if __name__ == "__main__":
    board = INITIAL_BOARD

    view = GUI()

    controller = Controller(board, view)

    view.show()
