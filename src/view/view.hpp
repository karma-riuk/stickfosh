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
    virtual Piece ask_about_promotion() = 0;
    virtual void update_board(const Board&, int8_t, std::vector<int8_t>) = 0;
    virtual void notify_checkmate(Colour) = 0;
    virtual void notify_stalemate(Colour) = 0;
};
