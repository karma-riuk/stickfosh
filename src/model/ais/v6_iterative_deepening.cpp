#include "../pieces/piece.hpp"
#include "../utils/threadpool.hpp"
#include "../utils/utils.hpp"
#include "ai.hpp"

#include <map>

static int position_counter = 0;

Move ai::v6_iterative_deepening::_search(const Board& b) {
    ThreadPool pool(std::thread::hardware_concurrency());
    std::vector<Move> moves = b.all_legal_moves();

    Move best_move;
    int best_eval = -INFINITY;

    std::map<Move, std::future<int>> futures;
    int depth;
    for (depth = 1; !stop_computation; depth++) {
        for (const Move& move : moves) {
            Board tmp_board = b.make_move(move);
            futures.insert(
                {move, pool.enqueue([&, tmp_board]() {
                     return _ab_search(tmp_board, depth, -INFINITY, INFINITY);
                 })}
            );
        }

        int counter = 0;
        for (auto& [move, future] : futures) {
            int eval = future.get();
            counter++;
            if (!am_white)
                eval *= -1;
            if (eval > best_eval) {
                best_eval = eval;
                best_move = move;
            }
        }
        futures.clear();
    }

    std::cout << "Went up until depth: " << depth << std::endl;
    return best_move;
}
