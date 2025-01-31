import tkinter as tk
from tkinter import messagebox
from typing import Type
from PIL import ImageTk, Image
import os

from logic.board import Board
from logic.move import Move
from logic.pieces.bishop import Bishop
from logic.pieces.king import King
from logic.pieces.knight import Knight
from logic.pieces.pawn import Pawn
from logic.pieces.piece import Colour, Piece
from logic.pieces.queen import Queen
from logic.pieces.rook import Rook
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

        self._piece_images = self._load_piece_images("res/")

    def _piece_svg(self, root: str, piece: Type[Piece], colour: Colour) -> ImageTk.PhotoImage:
        piece_name = piece.__name__.lower()

        path = os.path.join(root, f"{"white" if colour == Colour.WHITE else "black"}-{piece_name}.png")
        img = Image.open(path)

        if img.mode == "LA":
            img = img.convert(mode="RGBA")
            img.save(path)

        return ImageTk.PhotoImage(img)

    def _load_piece_images(self, root: str) -> dict[Type[Piece], dict[Colour, ImageTk.PhotoImage]]:
        ret = {}
        for piece in [Pawn, Rook, Knight, Bishop, Queen, King]:
            if piece not in ret:
                ret[piece] = {}
            ret[piece][Colour.WHITE] = self._piece_svg(root, piece, Colour.WHITE)
            ret[piece][Colour.BLACK] = self._piece_svg(root, piece, Colour.BLACK)

        return ret

    def _on_canvas_click(self, event):
        x, y = event.x // self.tile_size, event.y // self.tile_size
        y = 7 - y

        self._controller.on_tile_selected(x, y)

    def notify_checkmate(self, colour: Colour) -> None:
        messagebox.showinfo("Checkmate",  f"{colour} is currently checkmated") 
        
    def notify_stalemate(self, colour: Colour) -> None:
        messagebox.showinfo("Stalemate",  f"{colour} is currently stalemated") 
        

    def update_board(self, board: Board, selected_piece: Piece, legal_moves: list[Move]) -> None:
        self.canvas.delete("all")
        self._draw_chess_board(board, selected_piece, legal_moves)


    def _draw_chess_board(self, board: Board, selected_piece = None, legal_moves = []):
        colours = ["#EDD6B0", "#B88762"]  # Light and dark squares
        alt_colours = ["#F6EB72", "#DCC34B"]  # Light and dark squares, when selected
        circle_colours = ["#CCB897", "#9E7454"] # circles to show legal moves

        for y in range(8):
            for x in range(8):
                colour = colours[(x + y) % 2]
                pos = Position(x, 7-y)
                if selected_piece is not None and pos == selected_piece.pos:
                    colour = alt_colours[(x + y) % 2]

                self.canvas.create_rectangle(
                    x * self.tile_size, 
                    y * self.tile_size, 
                    (x + 1) * self.tile_size, 
                    (y + 1) * self.tile_size, 
                    fill=colour,
                    outline=colour,
                )

                if selected_piece is not None:
                    possible_positions = [move.pos for move in legal_moves] 
                    if pos in possible_positions:
                        colour = circle_colours[(x + y) % 2]
                        move = [move for move in legal_moves if move.pos == pos][0]
                        if move.is_capturing:
                            radius = .40 * self.tile_size
                            self.canvas.create_oval(
                                (x + .5) * self.tile_size - radius,
                                (y + .5) * self.tile_size - radius,
                                (x + .5) * self.tile_size + radius,
                                (y + .5) * self.tile_size + radius,
                                fill="",
                                outline=colour,
                                width=.075 * self.tile_size,
                            )
                        else:
                            radius = .15 * self.tile_size
                            self.canvas.create_oval(
                                (x + .5) * self.tile_size - radius,
                                (y + .5) * self.tile_size - radius,
                                (x + .5) * self.tile_size + radius,
                                (y + .5) * self.tile_size + radius,
                                fill=colour,
                                outline=colour,
                            )

                piece = board.piece_at(x, 7-y)

                if piece:
                    self.canvas.create_image(
                        (x + 0.5) * self.tile_size, 
                        (y + 0.9) * self.tile_size, 
                        image=self._piece_images[type(piece)][piece.colour],
                        anchor=tk.S,
                    )

                # Cell annotations
                text_colour = colours[(x + y + 1) % 2] # the other colour

                if x == 0: # numbers in the top left of the first column
                    self.canvas.create_text(
                        (x + .15) * self.tile_size, 
                        (y + .15) * self.tile_size, 
                        text=8-y, 
                        fill=text_colour, 
                        font=("Arial", 12, "bold")
                    )
                if y == 7: # numbers in the top left of the first column
                    self.canvas.create_text(
                        (x + .85) * self.tile_size, 
                        (y + .85) * self.tile_size, 
                        text="abcdefgh"[x], 
                        fill=text_colour, 
                        font=("Arial", 12, "bold")
                    )

    def show(self) -> None:
        self.root.mainloop()
