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

    class v1_simple : public AI { // looks two moves ahead
        int _search(const Board&, int);

      public:
        v1_simple(bool w, std::chrono::milliseconds tt): AI(w, tt) {}

        Move _search(const Board&) override;
        int eval(const Board&) override;
    };
} // namespace ai
