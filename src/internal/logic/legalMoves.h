#pragma once

#include <unordered_set>
#include <utility>
#include <vector>

#include "internal/logic/attackedSquares.h"
#include "internal/logic/move.h"
#include "internal/logic/offset.h"
#include "internal/logic/square.h"
#include "internal/logic/state.h"

namespace ChessGame
{
    std::unordered_set<Move> legalMoves(const State &state);
} // namespace ChessGame