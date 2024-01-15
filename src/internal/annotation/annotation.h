#pragma once

#include <optional>
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
    std::optional<std::string_view> toPGN(const Annotation annotation);
    std::optional<std::string_view> toStr(const std::optional<Annotation> annotation);
} // namespace ChessGame
