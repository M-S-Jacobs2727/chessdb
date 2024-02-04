#pragma once

#include <cstdint>

namespace JChess
{
    struct Evaluation
    {
        int32_t value = 0;
        bool centipawns = true;
    };
}