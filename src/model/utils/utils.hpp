#pragma once

#include "move.hpp"

#include <cstdint>
#include <limits>
#include <vector>

std::vector<int8_t> to_target_square(std::vector<Move>);
int count_material(const Board&, int8_t, bool = true);
int piece_value(Piece);

const int INFINITY = std::numeric_limits<int>::max();

const int PawnValue = 100;
const int KnightValue = 300;
const int BishopValue = 320;
const int RookValue = 500;
const int QueenValue = 900;
