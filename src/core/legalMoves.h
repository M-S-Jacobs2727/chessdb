#pragma once

#include <vector>

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
    std::vector<Move> legalMoves(const State &state);
} // namespace ChessGame