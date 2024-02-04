#pragma once

#include <cstdint>
#include <functional>
#include <optional>

#include "core/castling.h"
#include "core/piece.h"
#include "core/square.h"

namespace JChess
{
    struct Move
    {
        Piece piece;
        Square from;
        Square to;

        bool enPassant = false;
        std::optional<Piece> capture = std::nullopt;
        std::optional<Castling::Side> castle = std::nullopt;
        std::optional<Piece> promotion = std::nullopt;

        bool operator==(const Move &other) const
        {
            return piece == other.piece &&
                   from == other.from &&
                   to == other.to;
        }
    };
} // namespace ChessGame

template <>
struct std::hash<JChess::Move>
{
    inline size_t operator()(const JChess::Move &move) const noexcept
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
