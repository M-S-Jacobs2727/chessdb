#pragma once

#include <optional>
#include <vector>

#include "core/move.h"
#include "core/square.h"
#include "core/state.h"

namespace ChessGame
{
    std::optional<Square> getHardPin(const State &state, Square testSquare, Square kingSq);
    std::vector<Move> legalMoves(const State &state);
} // namespace ChessGame