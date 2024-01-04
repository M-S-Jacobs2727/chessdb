#include "result.h"

#include <array>

namespace ChessGame
{
    std::string_view ChessGame::toStr(const GameResult::Type t)
    {
        using namespace std::string_view_literals;
        std::array<std::string_view, 4> resultArr{"none"sv, "white"sv, "black"sv, "draw"sv};
        return resultArr[static_cast<int>(t)];
    }
    std::string_view ChessGame::toStr(const GameResult::Reason r)
    {
        using namespace std::string_view_literals;
        std::array<std::string_view, 8> conditionArr{
            "none"sv,
            "checkmate"sv,
            "timeout"sv,
            "resignation"sv,
            "infraction"sv,
            "stalemate"sv,
            "agreement"sv,
            "material"sv,
        };
        return conditionArr[static_cast<int>(r)];
    }
} // namespace ChessGame