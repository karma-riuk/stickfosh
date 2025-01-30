import tkinter as tk

from logic.board import Board
from logic.pieces.piece import Piece
from logic.position import Position
from view.view import View

class GUI(View):
    def __init__(self, board: Board) -> None:
        super().__init__(board)

        self.root = tk.Tk()
        self.root.title("Chess Board")

        self.tile_size = 80
        board_size = self.tile_size * 8

        self.canvas = tk.Canvas(self.root, width=board_size, height=board_size)
        self.canvas.pack()

        self.state = {"selected_piece": None, "legal_moves": []}

        self.canvas.bind("<Button-1>", self._on_canvas_click)
        self._draw_chess_board()


    def _draw_chess_board(self):
        colours = ["#F0D9B5", "#B58863"]  # Light and dark squares

        for y in range(8):
            for x in range(8):
                colour = colours[(x + y) % 2]
                if self.state["selected_piece"]:
                    possible_positions = [move.pos for move in self.state["legal_moves"]] 
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

                piece = self.board.piece_at(x, 7-y)

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
                    

    def _on_canvas_click(self, event):
        x, y = event.x // self.tile_size, event.y // self.tile_size
        y = 7 - y
        piece = self.board.piece_at(x, y)
        print(f"Clicked on {x, y}, {piece = }")

        if piece:
            self.state["selected_piece"] = piece
            self.state["legal_moves"] = piece.legal_moves(self.board)
        else:
            self.state["selected_piece"] = None
            self.state["legal_moves"] = []

        self.canvas.delete("all")
        self._draw_chess_board()

    def show(self) -> None:
        self.root.mainloop()
