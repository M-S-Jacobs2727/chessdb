#pragma once

#include <optional>

#include "internal/logic/square.h"

namespace ChessGame
{
    struct Offset
    {
        int file = 0;
        int rank = 0;

        constexpr Offset operator+(const Offset &other) const
        {
            return {
                .file = file + other.file,
                .rank = rank + other.rank,
            };
        }

        constexpr Offset operator*(int factor) const
        {
            return {
                .file = file * factor,
                .rank = rank * factor,
            };
        }

        constexpr Offset operator/(int factor) const
        {
            return {
                .file = file / factor,
                .rank = rank / factor,
            };
        }

        constexpr Offset operator-() const
        {
            return (*this) * -1;
        }

        constexpr Offset operator-(const Offset &other) const
        {
            return (*this) + (-other);
        }

        constexpr void operator+=(const Offset &other)
        {
            file += other.file;
            rank += other.rank;
        }

        constexpr void operator-=(const Offset &other)
        {
            file -= other.file;
            rank -= other.rank;
        }

        constexpr void operator*=(const int factor)
        {
            file *= factor;
            rank *= factor;
        }

        constexpr void operator/=(const int factor)
        {
            file /= factor;
            rank /= factor;
        }

        constexpr std::optional<Square> operator()(const Square &square) const
        {
            Square s{square.file + file,
                     square.rank + rank};
            return s.valid() ? std::make_optional(s) : std::nullopt;
        }
    };

    inline Offset forward(Color color)
    {
        return Offset{0, static_cast<int>(color) * 2 - 1};
    }

    inline Offset backward(Color color)
    {
        return Offset{0, static_cast<int>(color) * -2 + 1};
    }
}