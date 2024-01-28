#pragma once

#include <functional>
#include <optional>

#include "internal/logic/castling.h"
#include "internal/logic/piece.h"
#include "internal/logic/square.h"

namespace ChessGame
{
    struct Move
    {
        Piece piece;
        Square from;
        Square to;

        bool enPassant = false;
        std::optional<PieceType> capture = std::nullopt;
        std::optional<Castling::Side> castle = std::nullopt;
        std::optional<PieceType> promotion = std::nullopt;

        bool operator==(const Move &other) const
        {
            return piece == other.piece &&
                   from == other.from &&
                   to == other.to;
        }
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
        return h;
    }
};
