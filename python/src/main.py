import time
from pprint import pprint
from tqdm import tqdm

from ai.ai import move_generation_test
from controller.controller import Controller
from logic.board import INITIAL_BOARD, Board
from logic.position import Position
from view.gui import GUI
from view.tui import TUI

from ai.ai import peft

if __name__ == "__main__":
    board = INITIAL_BOARD

    pos = "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8"
    board = Board.setup_FEN_position(pos)

    view = GUI()

    controller = Controller(board, view)

    # view.show()
    # exit()

    peft(pos)
