#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

#include "internal/logic/color.h"

namespace ChessGame
{
    enum class PieceType
    {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,
    };

    struct Piece
    {
        Color color;
        PieceType type;
        inline bool operator==(const Piece &other) const
        {
            return type == other.type && color == other.color;
        }
    };

    constexpr inline uint8_t homeRank(const Color color)
    {
        return 7 * static_cast<int>(color);
    }
}