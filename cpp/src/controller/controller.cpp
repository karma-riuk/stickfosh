#include "controller.hpp"

#include "../view/view.hpp"

Controller::Controller(Board b, View& v): board(b), view(v) {
    v.set_controller(this);
}
