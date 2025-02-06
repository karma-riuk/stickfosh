#pragma once

#include "../board/board.hpp"

#include <atomic>

namespace ai {
    struct AI {
        std::atomic<bool> stop_computation = false;
        virtual Move search(const Board&, bool = false) = 0;
        virtual int eval(const Board&) = 0;
    };

    struct v0_random : public AI {
        Move search(const Board&, bool) override;
        // int eval(const Board&) override;
    };

    class v1_simple : public AI {
        int _search(const Board&, int);

      public:
        Move search(const Board&, bool) override;
        int eval(const Board&) override;
    };
} // namespace ai
