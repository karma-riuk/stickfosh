#pragma once

#include "board.hpp"

#include <string>

std::string search(std::string, int);
int minimax(const Board&, int);
int eval(const Board&);
