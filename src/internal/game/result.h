#pragma once

#include <string_view>

namespace ChessGame
{
    struct GameResult
    {
        enum class Type
        {
            None,
            WhiteWins,
            BlackWins,
            Draw,
        };
        enum class Reason
        {
            None,
            Checkmate,
            Timeout,
            Resignation,
            RulesInfraction,
            Stalemate,
            Agreement,
            InsufficientMaterial,
        };
        Type type;
        Reason reason;
    };

    std::string_view toStr(const GameResult::Type t);
    std::string_view toStr(const GameResult::Reason r);
} // namespace ChessGame
