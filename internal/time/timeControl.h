#pragma once

#include <cstdint>
#include <string>

namespace ChessGame
{
    struct TimeControl
    {
        uint16_t initialTime;
        uint16_t increment;

        TimeControl() = default;
        ~TimeControl() = default;
        TimeControl(uint16_t initialTime, uint16_t increment) : initialTime(initialTime), increment(increment) {}
        explicit TimeControl(std::string &timeControlString)
        {
            auto idx = timeControlString.find('+');
            initialTime = std::stoul(timeControlString.substr(0, idx));
            increment = std::stoul(timeControlString.substr(idx + 1));
        }
    };
} // namespace ChessGame
