import unittest

import sys
sys.path.append('src') # you must execute pytest from the stickfosh dir for this to work

from logic.position import Position

class PositionTests(unittest.TestCase):
    def testXY2Algebraic(self):
        self.assertEqual(Position(0, 0).to_algebraic(), "a1")
        self.assertEqual(Position(1, 0).to_algebraic(), "b1")

        self.assertEqual(Position(2, 1).to_algebraic(), "c2")
        self.assertEqual(Position(4, 2).to_algebraic(), "e3")

        self.assertEqual(Position(7, 7).to_algebraic(), "h8")

    def testAlgebraic2XY(self):
        self.assertEqual(Position.from_algebraic("a1"), Position(0, 0))
        self.assertEqual(Position.from_algebraic("b1"), Position(1, 0))

        self.assertEqual(Position.from_algebraic("c2"), Position(2, 1))
        self.assertEqual(Position.from_algebraic("e3"), Position(4, 2))

        self.assertEqual(Position.from_algebraic("h8"), Position(7, 7))

        self.assertRaises(AssertionError, lambda : Position.from_algebraic("a11"))

        self.assertRaises(ValueError, lambda : Position.from_algebraic("j1"))
        self.assertRaises(ValueError, lambda : Position.from_algebraic("a9"))
