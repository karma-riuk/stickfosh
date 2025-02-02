#include "../board.cpp"

#define IS_TRUE(x)                                                             \
    {                                                                          \
        if (!(x))                                                              \
            std::cout << __FUNCTION__ << " failed on line " << __LINE__        \
                      << std::endl;                                            \
    }

#define ASSERT_EQUALS(expected, actual)                                        \
    {                                                                          \
        if (expected != actual)                                                \
            std::cout << "Expected: " << std::endl                             \
                      << '\t' << expected << std::endl                         \
                      << "Got: " << std::endl                                  \
                      << '\t' << actual << std::endl;                          \
    }

int main() {
    std::string pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());

    pos = "r1bk3r/p2pBpNp/n4n2/1p1NP2P/6P1/3P4/P1P1K3/q5b1";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());

    pos = "rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());

    pos = "4k2r/6r1/8/8/8/8/3R4/R3K3";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());

    pos = "8/8/8/4p1K1/2k1P3/8/8/8";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());

    pos = "8/5k2/3p4/1p1Pp2p/pP2Pp1P/P4P1K/8/8";
    ASSERT_EQUALS(pos, Board::setup_fen_position(pos)->to_fen());
}
