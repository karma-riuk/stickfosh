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

    pos = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
    board = Board.setup_FEN_position(pos)

    view = GUI()

    controller = Controller(board, view)

    # view.show()
    # exit()

    peft(pos)
