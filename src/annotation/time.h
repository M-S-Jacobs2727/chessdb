#pragma once

#include <chrono>
#include <cstdint>

namespace JChess
{
    using msDuration = std::chrono::duration<uint32_t, std::milli>;
    struct TimeControl
    {
        msDuration initial;
        msDuration increment;
    };
}