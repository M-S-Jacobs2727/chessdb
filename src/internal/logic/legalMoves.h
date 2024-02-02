#pragma once

#include <unordered_set>

#include "internal/logic/offset.h"
#include "internal/logic/square.h"
#include "internal/logic/state.h"

namespace ChessGame
{
    struct HardPin
    {
        Square attacker;
        Offset direction;
    };
    std::unordered_set<Move> legalMoves(const State &state);
} // namespace ChessGame