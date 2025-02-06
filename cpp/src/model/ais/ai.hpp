#pragma once

#include "../board/board.hpp"

#include <atomic>
#include <chrono>

namespace ai {
    class AI {
      protected:
        std::chrono::milliseconds thinking_time;
        virtual Move _search(const Board&, bool = false) = 0;

      public:
        AI(std::chrono::milliseconds tt): thinking_time(tt) {}

        std::atomic<bool> stop_computation = false;

        Move search(const Board& b, bool am_white = false);

        virtual int eval(const Board&) = 0;
    };

    struct v0_random : public AI {
        v0_random(std::chrono::milliseconds tt): AI(tt) {}

        Move _search(const Board&, bool) override;

        int eval(const Board&) override {
            return 0;
        };
    };

    class v1_simple : public AI {
        int _search(const Board&, int);

      public:
        v1_simple(std::chrono::milliseconds tt): AI(tt) {}

        Move _search(const Board&, bool) override;
        int eval(const Board&) override;
    };
} // namespace ai
