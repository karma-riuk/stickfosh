#pragma once

#include "../board/board.hpp"

#include <atomic>
#include <chrono>

namespace ai {
    class AI {
      protected:
        bool am_white;
        std::chrono::milliseconds thinking_time;
        virtual Move _search(const Board&) = 0;

      public:
        AI(bool am_white, std::chrono::milliseconds tt)
            : am_white(am_white),
              thinking_time(tt) {}

        std::atomic<bool> stop_computation = false;

        Move search(const Board& b);

        virtual int eval(const Board&) = 0;
    };

    struct v0_random : public AI {
        v0_random(bool w, std::chrono::milliseconds tt): AI(w, tt) {}

        Move _search(const Board&) override;

        int eval(const Board&) override {
            return 0;
        };
    };

    class v1_pure_minimax : public AI { // looks two moves ahead
        int _search(const Board&, int);

      public:
        v1_pure_minimax(bool w, std::chrono::milliseconds tt): AI(w, tt) {}

        Move _search(const Board&) override;
        int eval(const Board&) override;
    };

    class v2_alpha_beta : public AI {
        // looks two moves ahead, with alpha-beta pruning (no move ordering)
        int _search(const Board&, int, int, int);

      public:
        v2_alpha_beta(bool w, std::chrono::milliseconds tt): AI(w, tt) {}

        Move _search(const Board&) override;
        int eval(const Board&) override;
    };

    class v3_AB_ordering : public AI {
        // looks two moves ahead, with alpha-beta pruning, with move ordering
        virtual int _search(const Board&, int, int, int);

      public:
        v3_AB_ordering(bool w, std::chrono::milliseconds tt): AI(w, tt) {}

        Move _search(const Board&) override;
        int eval(const Board&) override;
    };

    class v4_search_captures : public v3_AB_ordering {
        // same as v3, but looking at only at captures when leaf is reached,
        // until no captures are left
        int _search(const Board&, int, int, int) override;
        int _search_captures(const Board&, int, int);

      public:
        v4_search_captures(bool w, std::chrono::milliseconds tt)
            : v3_AB_ordering(w, tt) {}
    };
} // namespace ai
