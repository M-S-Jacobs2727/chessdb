#pragma once

#include <string>

#include "internal/annotation.h"
#include "piece.h"
#include "square.h"

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

        Move() = default;
        Move(const std::string_view &str, Color color);
    };
} // namespace ChessGame
