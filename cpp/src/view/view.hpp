#pragma once

#include "../controller/controller.hpp"
#include "../model/board/board.hpp"
#include "../model/pieces/piece.hpp"
#include "../model/utils/move.hpp"

class View {
  protected:
    Controller* controller;

  public:
    void set_controller(Controller* c) {
        controller = c;
    }

    virtual void show() = 0;
    virtual void update_board(const Board&, int8_t, std::vector<Move>) = 0;
    virtual void notify_checkmate() = 0;
    virtual void notify_stalemate() = 0;
};
