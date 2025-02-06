#include "../model/board/board.hpp"
#include "view.hpp"

class NoOpView : public View {
  public:
    NoOpView() {};

    void show() override {};
    void update_board(const Board&, int8_t, std::vector<int8_t>) override {};
    void notify_checkmate(Colour) override{};
    void notify_stalemate(Colour) override{};
};
