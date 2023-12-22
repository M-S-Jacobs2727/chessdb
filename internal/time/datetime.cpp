#include "datetime.h"

namespace ChessGame
{
    Datetime::Datetime(const std::string &dateString, const std::string &timeString)
    {
        std::regex date_regex(R"((\d{4})[.-/]?(\d{1,2})[.-/]?(\d{1,2}))");
        std::regex time_regex(R"((\d{1,2})[:.]?(\d{2})[:.]?(\d{2}))");
        std::smatch matchDate, matchTime;

        if (!std::regex_match(dateString, matchDate, date_regex))
            throw std::runtime_error("Invalid date string");
        if (!std::regex_match(timeString, matchTime, time_regex))
            throw std::runtime_error("Invalid date string");

        year = std::stoul(matchDate[1].str());
        month = std::stoul(matchDate[2].str());
        day = std::stoul(matchDate[3].str());

        hour = std::stoul(matchTime[1].str());
        minute = std::stoul(matchTime[2].str());
        second = std::stoul(matchTime[3].str());
    }
} // namespace ChessGame
