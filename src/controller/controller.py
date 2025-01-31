from logic.board import Board
from logic.move import Move
from logic.pieces.piece import Piece
from logic.position import Position
from view.view import View


class Controller:
    def __init__(self, board: Board, view: View) -> None:
        self._board = board
        self._view = view

        self._view.set_controller(self)
        self._reset_selection()

        self._selected_piece: Piece = None
        self._legal_moves: list[Move] = []

    def _reset_selection(self):
        self._selected_piece = None
        self._legal_moves = []
        self._view.update_board(self._board, self._selected_piece, self._legal_moves)


    def _show_legal_moves(self, pos: Position):
        piece = self._board.piece_at(pos.x, pos.y)

        if piece:
            if piece.colour != self._board._turn:
                return
            self._selected_piece = piece
            self._legal_moves = piece.legal_moves(self._board)
            self._view.update_board(self._board, self._selected_piece, self._legal_moves)
        else:
            self._reset_selection()

    def _make_move(self, move: Move) -> None:
        self._board = self._board.make_move(move)
        self._reset_selection()

    def on_tile_selected(self, x: int, y: int) -> None:
        pos = Position(x, y)
        print(f"Clicked on {pos.to_algebraic()}")

        piece = self._board.piece_at(x, y)

        if self._selected_piece is None or (piece is not None and piece != self._selected_piece):
            self._show_legal_moves(pos)
        else:
            legal_moves_positions = [move for move in self._legal_moves if move.pos == pos]
            assert len(legal_moves_positions) <= 1, f"Apparently we can make multiple moves towards {pos.to_algebraic()} with {type(self._selected_piece)}, which doesn't make sense..."

            if len(legal_moves_positions) == 0: # click on a square outside of the possible moves
                self._reset_selection()
            else:
                move = legal_moves_positions[0]
                self._make_move(move)
