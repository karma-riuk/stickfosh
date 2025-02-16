#include "../src/model/board/board.hpp"
#include "lib.hpp"

int main() {
    std::string str_move = "a2a3";
    ASSERT_EQUALS(str_move, Move::from_string(str_move).to_string());

    str_move = "b2f4";
    ASSERT_EQUALS(str_move, Move::from_string(str_move).to_string());

    str_move = "a2a1r";
    ASSERT_EQUALS(str_move, Move::from_string(str_move).to_string());
}
