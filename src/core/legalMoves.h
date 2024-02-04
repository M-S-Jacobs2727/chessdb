#pragma once

#include <vector>

#include "core/move.h"
#include "core/offset.h"
#include "core/square.h"
#include "core/state.h"

namespace ChessGame
{
    std::vector<Move> legalMoves(const State &state);
} // namespace ChessGame