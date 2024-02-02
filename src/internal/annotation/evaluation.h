#pragma once

#include <cstdint>

namespace ChessGame
{
    struct Evaluation
    {
        int32_t value = 0;
        bool centipawns = true;
    };
}