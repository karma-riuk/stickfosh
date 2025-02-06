#pragma once

#include "../board.hpp"

#include <string>

#define DECLARE_AI(x)                                                          \
    struct x : public AI {                                                     \
        std::string search(std::string, int) override;                         \
        int minimax(const Board&, int) override;                               \
        int eval(const Board&) override;                                       \
    };

namespace ai {
    struct AI {
        virtual std::string search(std::string, int) = 0;
        virtual int minimax(const Board&, int) = 0;
        virtual int eval(const Board&) = 0;
    };

    DECLARE_AI(v0_random)
    DECLARE_AI(v1_simple)
} // namespace ai
