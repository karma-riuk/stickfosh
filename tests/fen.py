import unittest

import sys
sys.path.append('src') # you must execute pytest from the stickfosh dir for this to work

from logic.board import Board

class FENTests(unittest.TestCase):
    def testInitialPosition(self):
        pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())

    def testRandomPositions(self):
        pos = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1 b Qk - 0 1"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())

        pos = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq e3 0 1"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())

        pos = "4k2r/6r1/8/8/8/8/3R4/R3K3 w Qk - 0 1"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())

        pos = "8/8/8/4p1K1/2k1P3/8/8/8 b - - 0 1"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())

        pos = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8 b - - 99 50"
        self.assertEqual(pos, Board.setup_FEN_position(pos).to_fen_string())



