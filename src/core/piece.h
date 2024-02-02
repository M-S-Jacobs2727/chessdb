#pragma once

#include "core/color.h"

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
}