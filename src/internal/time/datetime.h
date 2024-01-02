#pragma once

#include <cstdint>
#include <regex>
#include <string>

namespace ChessGame
{
    struct Datetime
    {
        uint16_t year = 0u;
        uint8_t month = 0u;
        uint8_t day = 0u;
        uint8_t hour = 0u;
        uint8_t minute = 0u;
        uint8_t second = 0u;

        Datetime() = default;
        Datetime(const std::string &dateString, const std::string &timeString);
    };
} // namespace ChessGame
