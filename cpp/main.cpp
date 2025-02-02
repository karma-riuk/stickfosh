#include "board.cpp"

int main(int argc, char* argv[]) {
    std::string pos =
        "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8";
    Board* b = Board::setup_fen_position(pos);

    std::string fen = b->to_fen();

    std::cout << pos << std::endl;
    std::cout << fen << std::endl;

    std::cout << (fen.substr(0, fen.find(" ")) == pos.substr(0, pos.find(" ")))
              << std::endl;
    return 0;
}
