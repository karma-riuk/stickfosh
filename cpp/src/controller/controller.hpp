#pragma once

#include "../model/board/board.hpp"

class View;

class Controller {
  protected:
    Board board;

  public:
    // Controller(Board, View);
    virtual void on_tile_selected(int, int) = 0;
};
