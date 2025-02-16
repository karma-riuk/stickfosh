#pragma once

#include "../model/ais/ai.hpp"
#include "../view/view.hpp"
#include "manual.hpp"

class HumanVsAIController : public ManualController {
  protected:
    ai::AI& ai;

  public:
    HumanVsAIController(Board, View&, ai::AI&);
    void on_tile_selected(int, int) override;
};
