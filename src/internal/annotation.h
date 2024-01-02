#pragma once

#include <string_view>

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

    Annotation readAnnotation(const std::string_view &str);
} // namespace ChessGame
