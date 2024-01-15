#include <cstdlib>
#include <regex>
#include <string_view>

#include "clockTime.h"

namespace ChessGame
{
    ClockTime::ClockTime(std::string_view timeString)
    {
        std::regex r(R"((\d+):(\d{2}):(\d{2}))");
        std::cmatch match;

        if (!std::regex_match(timeString.data(), match, r))
            throw std::runtime_error("invalid clock string");

        seconds = 3600 * std::stoul(match[1].str());
        seconds += 60 * std::stoul(match[2].str());
        seconds += std::stoul(match[3].str());
    }
} // namespace ChessGame
