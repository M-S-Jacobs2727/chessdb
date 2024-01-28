#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

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

    enum class Color
    {
        White,
        Black,
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

    constexpr inline Color oppositeColor(const Color color)
    {
        return static_cast<Color>(1 - static_cast<int>(color));
    }

    PieceType readPGNPieceType(std::string_view moveStr);
    std::string_view toPGN(const PieceType pt);
    std::string_view toFEN(const PieceType pt, const Color color);
    std::optional<std::string_view> toStr(const PieceType pt);
    constexpr std::string_view toStr(const Color c);

    constexpr inline uint8_t homeRank(const Color color)
    {
        return -7 * (static_cast<int>(color) - 1);
    }
}