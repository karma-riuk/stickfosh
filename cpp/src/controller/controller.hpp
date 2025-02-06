#pragma once

#include "../model/board/board.hpp"

class View;

class Controller {
  protected:
    Board board;
    View& view;
    virtual void make_move(Move) = 0;

  public:
    Controller(Board, View&);
    virtual void start() = 0;
    virtual void on_tile_selected(int, int) = 0;
};
