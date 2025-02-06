#pragma once

#include "../model/ais/ai.hpp"
#include "../model/utils/coords.hpp"
#include "../model/utils/move.hpp"
#include "../view/view.hpp"
#include "controller.hpp"

class AIvsAIController : public Controller {
    ai::AI &p1, &p2;

  protected:
    void make_move(Move) override;

  public:
    AIvsAIController(Board, View&, ai::AI&, ai::AI&);
    void on_tile_selected(int, int) override;
    void start() override;
};
