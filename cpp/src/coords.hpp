#pragma once

#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <string>

static std::string _FILES = "abcdefgh";
static std::string _RANKS = "12345678";

struct Coords {
    int x, y;

    Coords(int x, int y): x(x), y(y) {}

    int8_t to_index() const {
        return this->y * 8 + this->x;
    }

    static Coords from_index(int idx, const char* yes = __builtin_FUNCTION()) {
        // std::cout << yes << std::endl;
        if (idx < 0 || idx > 63)
            throw std::invalid_argument("The index is outside the board...");
        return {idx % 8, idx / 8};
    }

    static Coords from_algebraic(std::string pos) {
        if (pos.size() != 2)
            throw std::invalid_argument(
                "An algebraic coordinate should only have two characters"
            );

        int x = _FILES.find(pos[0]);
        if (x == std::string::npos)
            throw std::invalid_argument("The first character of the given "
                                        "algebraic coordinate is invalid");

        int y = _RANKS.find(pos[1]);
        if (y == std::string::npos)
            throw std::invalid_argument("The second character of the given "
                                        "algebraic coordinate is invalid");

        return Coords{x, y};
    }

    std::string to_algebraic() const {
        std::string ret;
        if (x > 7 || y > 7)
            throw std::invalid_argument(
                "Can't give the algebraic vesion of an invalid coord"
            );
        ret += _FILES[x];
        ret += _RANKS[y];
        return ret;
    }

    bool is_within_bounds() const {
        return 0 <= x && x < 8 && 0 <= y && y < 8;
    }
};

inline bool operator==(const Coords& a, const Coords& b) {
    return a.x == b.x && a.y == b.y;
}

inline bool operator!=(const Coords& a, const Coords& b) {
    return !(a == b);
}

inline std::ostream& operator<<(std::ostream& os, const Coords& coords) {
    os << coords.to_algebraic();
    return os;
}
