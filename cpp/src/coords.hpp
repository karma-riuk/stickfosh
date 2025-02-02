#pragma once

#include <cstdint>

struct Coords {
    int x, y;

    int8_t to_index() const {
        return this->y * 8 + this->x;
    }

    static Coords from_index(int idx) {
        return {idx % 8, idx / 8};
    }

    bool is_within_bounds() const {
        return this->to_index() < 64;
    }
};
