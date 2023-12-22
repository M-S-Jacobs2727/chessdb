#pragma once

#include <string>

#include "piece.h"
#include "square.h"

namespace ChessGame
{
    enum class Annotation
    {
        None,
        Good,
        Mistake,
        Brilliant,
        Blunder,
        Interesting,
        Dubious,
        None2,
    };

    struct Move
    {
        std::string moveString;
        Piece piece;
        Square from;
        Square to;
        Piece capturedPiece;
        bool check = false;
        Annotation annotation;
        Piece promotedPiece;
        std::string_view partialFrom = "";

        Move() = default;
        Move(const std::string_view &str, Color color);
    };

    Annotation readAnnotation(const std::string_view &str);
} // namespace ChessGame
