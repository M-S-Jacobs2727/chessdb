#pragma once

#include <cstdint>
#include <string_view>

namespace ChessGame
{
    struct ClockTime
    {
        ClockTime() = default;
        explicit ClockTime(std::string_view);

        uint16_t seconds = 0;
    };
} // namespace ChessGame
