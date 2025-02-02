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

    # -- Position 3
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - 0 1": {
        1: 14,
        2: 191,
        3: 2_812,
        4: 43_238,
        5: 674_624,
    },

    # -- Position 4
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1": {
        1: 6,
        2: 264,
        3: 9467,
        4: 422_333,
    },

    # -- Position 5
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8": {
        1: 44,
        2: 1486,
        3: 62379,
        4: 2103487,
    },

    # -- Position 6
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10": {
        1: 46,
        2: 2079,
        3: 89890,
        4: 3894594,
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
