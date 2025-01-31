import tkinter as tk

from logic.board import Board
from logic.move import Move
from logic.pieces.piece import Colour, Piece
from logic.position import Position
from view.view import View

class GUI(View):
    def __init__(self) -> None:
        super().__init__()

        self.root = tk.Tk()
        self.root.title("Chess Board")

        self.tile_size = 80
        board_size = self.tile_size * 8

        self.canvas = tk.Canvas(self.root, width=board_size, height=board_size)
        self.canvas.pack()

        self.canvas.bind("<Button-1>", self._on_canvas_click)

    def _on_canvas_click(self, event):
        x, y = event.x // self.tile_size, event.y // self.tile_size
        y = 7 - y

        self._controller.on_tile_selected(x, y)

    def update_board(self, board: Board, selected_piece: Piece, legal_moves: list[Move]) -> None:
        self.canvas.delete("all")
        self._draw_chess_board(board, selected_piece, legal_moves)

    def _draw_chess_board(self, board, selected_piece = None, legal_moves = []):
        colours = ["#F0D9B5", "#B58863"]  # Light and dark squares

        for y in range(8):
            for x in range(8):
                colour = colours[(x + y) % 2]
                if selected_piece is not None:
                    possible_positions = [move.pos for move in legal_moves] 
                    if Position(x, 7-y) in possible_positions:
                        colour = "#ADD8E6"  # Highlight legal moves

                self.canvas.create_rectangle(
                    x * self.tile_size, 
                    y * self.tile_size, 
                    (x + 1) * self.tile_size, 
                    (y + 1) * self.tile_size, 
                    fill=colour,
                    outline=colour,
                )

                piece = board.piece_at(x, 7-y)

                if piece:
                    text_colour = "white" if piece.colour == Colour.WHITE else "black"
                    self.canvas.create_text(
                        (x + 0.5) * self.tile_size, 
                        (y + 0.5) * self.tile_size, 
                        text=piece.__class__.__name__[0], 
                        fill=text_colour, 
                        font=("Arial", 32, "bold")
                    )

                # Cell annotations
                text_colour = colours[(x + y + 1) % 2] # the other colour

                if x == 0: # numbers in the top left of the first column
                    self.canvas.create_text(
                        (x + 0.15) * self.tile_size, 
                        (y + 0.15) * self.tile_size, 
                        text=8-y, 
                        fill=text_colour, 
                        font=("Arial", 10, "bold")
                    )
                if y == 7: # numbers in the top left of the first column
                    self.canvas.create_text(
                        (x + 0.85) * self.tile_size, 
                        (y + 0.85) * self.tile_size, 
                        text="abcdefgh"[x], 
                        fill=text_colour, 
                        font=("Arial", 10, "bold")
                    )

    def show(self) -> None:
        self.root.mainloop()
