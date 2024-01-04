#pragma once

#include <cstdint>
#include <optional>
#include <string_view>

namespace ChessGame
{
    enum class PieceType
    {
        None,
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King,
        Any,
    };

    enum class Color
    {
        White,
        Black,
        Any
    };

    struct Piece
    {
        Color color;
        PieceType type;
        operator bool() const
        {
            return type != PieceType::None;
        }
        bool operator==(const Piece &other) const
        {
            return (type == PieceType::None && type == other.type) ||
                   (type == other.type && color == other.color);
        }
    };

    constexpr inline Color oppositeColor(const Color color)
    {
        return (color == Color::White) ? Color::Black : Color::White;
    }

    PieceType readPGNPieceType(const std::string_view &moveStr);
    std::string_view toPGN(const PieceType pt);
    std::string_view toFEN(const PieceType pt, const Color color = Color::White);
    std::optional<std::string_view> toStr(const PieceType pt);
    constexpr std::string_view toStr(const Color c);
}