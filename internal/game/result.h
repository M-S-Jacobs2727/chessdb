#pragma once

namespace ChessGame
{
    struct GameResult
    {
        enum class Type
        {
            WhiteWins,
            BlackWins,
            Draw,
            Other
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
} // namespace ChessGame
