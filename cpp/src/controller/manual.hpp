#pragma once

#include "../model/utils/coords.hpp"
#include "../model/utils/move.hpp"
#include "../view/view.hpp"
#include "controller.hpp"

class ManualController : public Controller {
  private:
    View& view;
    int8_t selected_index;
    Piece selected_piece;
    std::vector<int8_t> targets;

    void reset_selection();
    void show_legal_moves(Coords);
    void make_move(Move);

  public:
    ManualController(Board, View&);
    void on_tile_selected(int, int) override;
};
