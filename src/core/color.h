#pragma once

namespace JChess
{
    enum class Color
    {
        White,
        Black,
    };

    constexpr inline Color oppositeColor(const Color color)
    {
        return static_cast<Color>(1 - static_cast<int>(color));
    }
}