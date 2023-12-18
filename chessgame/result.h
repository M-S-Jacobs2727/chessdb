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
            Normal,
            Timeout,
            RulesInfraction,
            InsufficientMaterial,
            Unknown,
            Unknown2,
            Unknown3,
            Unknown4,
        };
        Type type;
        Reason reason;
    };
} // namespace ChessGame
