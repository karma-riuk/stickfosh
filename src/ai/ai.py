from collections import defaultdict
import time

from tqdm import tqdm
from logic.board import INITIAL_BOARD, Board
from logic.move import Move
from logic.pieces.piece import Colour

pos2expected = {
    # -- Position 1
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1": {
        1: 20,
        2: 400,
        3: 8_902,
        4: 197_281,
    },

    # -- Position 2
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1": {
        1: 48,
        2: 2_039,
        3: 97_862,
        4: 4_085_603,
    },
    # pos 2 after f3f6
    "r3k2r/p1ppqpb1/bn2pQp1/3PN3/1p2P3/2N4p/PPPBBPPP/R3K2R b KQkq - 0 1": {
        2: 2_111,
        3: 77_838,
    },
    # pos 2 after f3f6 and e7d8
    "r2qk2r/p1pp1pb1/bn2pQp1/3PN3/1p2P3/2N4p/PPPBBPPP/R3K2R w KQkq - 1 2": {
        2: 1843,
    },
}

tick = "\033[92m✔️\033[0m"  # Green Tick
cross = "\033[91m❌\033[0m"  # Red Cross

res = defaultdict(lambda : 0)

def peft(pos: str):
    global res
    expected = pos2expected[pos]
    board = Board.setup_FEN_position(pos)
    for depth in expected:
        with tqdm(total=expected[depth], desc=f"Depth: {depth}") as bar:
            start = time.process_time()
            moves = move_generation_test(bar, board, depth, depth)
            bar.close()
            elapsed = time.process_time() - start
            elapsed *= 1_000

            print("Depth:", depth, end=" ")
            print("Result:", moves, end=" ")
            if moves == expected[depth]:
                print(f"{tick}", end=" ")
            else:
                print(f"{cross} (expected {expected[depth]})", end=" ")
            print("positions Time:", int(elapsed), "milliseconds")

            if moves != expected[depth]:
                print()
                for key, value in res.items():
                    print(f"{key}: {value}")



def move_generation_test(bar, board: Board, depth: int, max_depth, first = None):
    global res
    if first is None:
        res = defaultdict(lambda : 0)

    if board.is_terminal():
        # bar.update(1)
        # res[first] += 1
        return 0

    if depth == 0:
        res[first] += 1
        bar.update(1)
        return 1

    moves = board.legal_moves()
    if depth == 1:
        res[first] += len(moves)
        bar.update(len(moves))
        return len(moves)

    num_pos = 0
    for move in moves:
        tmp_board = board.make_move(move)
        if first is None:
            first = move.piece.pos.to_algebraic() + move.pos.to_algebraic()

        if first == "f7h8":
            print(tmp_board.legal_moves())
        num_pos += move_generation_test(bar, tmp_board, depth - 1, max_depth, first = first)

        if depth == max_depth:
            first = None

    return num_pos



def play_game(board: Board, strategies: dict, verbose: bool = False) -> Board:
    """Play a turn-taking game. `strategies` is a {player_name: function} dict,
    where function(state) is used to get the player's move."""
    state = board
    move_counter = 1
    while not (board.is_checkmate_for(board._turn) or board.is_stalemate_for(board._turn)):
        player = board._turn
        move = strategies[player](state)
        state = board.make_move(move)
        if verbose:
            if player == Colour.WHITE:
                print(str(move_counter) + ".", move, end=" ")
            else:
                print(move)

    return state

def minmax_search(state: Board) -> tuple[float, Move]:
    """Search game tree to determine best move; return (value, move) pair."""
    return _max_value(state) if state._turn == Colour.WHITE else _min_value(state)

def _max_value(state: Board) -> tuple[float, Move]:
    if state.is_terminal():
        return state.utility(), None
    v, move = -float("inf"), None
    for a in state.legal_moves():
        v2, _ = _min_value(state.make_move(a))
        if v2 > v:
            v, move = v2, a
    return v, move

def _min_value(state: Board) -> tuple[float, Move]:
    if state.is_terminal():
        return state.utility(), None
    v, move = float("inf"), None
    for a in state.legal_moves():
        v2, _ = _min_value(state.make_move(a))
        if v2 < v:
            v, move = v2, a
    return v, move
