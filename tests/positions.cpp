#include "../src/model/utils/coords.hpp"
#include "lib.hpp"

int main() {
    ASSERT_EQUALS(Coords(0, 0).to_algebraic(), "a1");
    ASSERT_EQUALS(Coords(1, 0).to_algebraic(), "b1");

    ASSERT_EQUALS(Coords(2, 1).to_algebraic(), "c2");
    ASSERT_EQUALS(Coords(4, 2).to_algebraic(), "e3");

    ASSERT_EQUALS(Coords(7, 7).to_algebraic(), "h8");

    ASSERT_EQUALS(Coords::from_algebraic("a1"), Coords(0, 0));
    ASSERT_EQUALS(Coords::from_algebraic("b1"), Coords(1, 0));

    ASSERT_EQUALS(Coords::from_algebraic("c2"), Coords(2, 1));
    ASSERT_EQUALS(Coords::from_algebraic("e3"), Coords(4, 2));

    ASSERT_EQUALS(Coords::from_algebraic("h8"), Coords(7, 7));
}
