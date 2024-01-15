#pragma once

#include <functional>
#include <optional>

#include "internal/logic/piece.h"
#include "internal/logic/square.h"

namespace ChessGame
{
    struct Move
    {
        enum class PromotedPiece
        {
            KNIGHT,
            BISHOP,
            ROOK,
            QUEEN
        };

        enum class CastleSide
        {
            KING,
            QUEEN
        };

        Piece piece;
        Square from;
        Square to;
        std::optional<PromotedPiece> promotion = std::nullopt;
        std::optional<CastleSide> castle = std::nullopt;
    };
} // namespace ChessGame

template <>
struct std::hash<ChessGame::Move>
{
    inline size_t operator()(const ChessGame::Move &move) const noexcept
    {
        size_t h = 0;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.piece.color));
        h <<= 3;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.piece.type));
        h <<= 4;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.from.file));
        h <<= 4;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.from.rank));
        h <<= 4;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.to.file));
        h <<= 4;
        h ^= std::hash<size_t>{}(static_cast<size_t>(move.to.rank));
    }
};
