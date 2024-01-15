#pragma once

#include <functional>
#include <string>

#include "internal/annotation.h"
#include "internal/game/piece.h"
#include "internal/game/square.h"

namespace ChessGame
{
    struct Move
    {
        std::string moveString;
        Piece piece;
        Square from;
        Square to;
        Piece capturedPiece;
        bool check = false;
        Piece promotedPiece;
        std::string_view partialFrom = "";
        bool isCastle = false;

        Move() = default;
        Move(const std::string_view &str, Color color);
        bool operator==(const Move &other) const;
    };

} // namespace ChessGame

template <>
struct std::hash<ChessGame::Move>
{
    inline size_t operator()(const ChessGame::Move &move) const noexcept
    {
        auto h = std::hash<uint64_t>{}(static_cast<uint64_t>(move.piece.color));
        h <<= 3;
        h ^= std::hash<uint64_t>{}(static_cast<uint64_t>(move.piece.type));
        h <<= 4;
        h ^= std::hash<uint64_t>{}(static_cast<uint64_t>(move.from.file));
        h <<= 4;
        h ^= std::hash<uint64_t>{}(static_cast<uint64_t>(move.from.rank));
        h <<= 4;
        h ^= std::hash<uint64_t>{}(static_cast<uint64_t>(move.to.file));
        h <<= 4;
        h ^= std::hash<uint64_t>{}(static_cast<uint64_t>(move.to.rank));
    }
};
