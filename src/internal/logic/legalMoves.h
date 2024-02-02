#pragma once

#include <unordered_set>

#include "internal/logic/state.h"

namespace ChessGame
{
    std::unordered_set<Move> legalMoves(const State &state);
} // namespace ChessGame