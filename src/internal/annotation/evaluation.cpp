#include <format>
#include <regex>

#include "evaluation.h"

namespace ChessGame
{
    Evaluation::Evaluation(std::string_view evalString)
    {
        std::regex cenitpawn_regex(R"(-?\d+\.\d+)");
        std::regex mate_regex(R"(#-?\d+)");
        std::cmatch match;
        if (std::regex_match(evalString.data(), match, cenitpawn_regex))
            value = std::stof(match[0].str());
        else if (std::regex_match(evalString.data(), match, mate_regex))
            value = std::stoi(match[0].str().substr(1));
        else
            throw std::runtime_error("invalid evaluation string");
    }

    std::string_view Evaluation::str()
    {
        if (value.index() == 0) // int, mate
        {
            int v = std::get<int>(value);
            return std::format("M{}", v);
        }
        // float, centipawns
        int v = std::get<float>(value);
        return std::format("{:.2f}", v);
    }
} // namespace ChessGame
