#pragma once

#include <unordered_set>

#include "core/move.h"
#include "core/offset.h"
#include "core/square.h"
#include "core/state.h"

namespace ChessGame
{
    struct HardPin
    {
        Square attacker;
        Offset direction;
    };
    // TODO: change unordered_set to vector
    std::unordered_set<Move> legalMoves(const State &state);
} // namespace ChessGame