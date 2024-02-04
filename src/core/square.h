#pragma once

#include <cstdint>
#include <functional>

#include "core/offset.h"

namespace JChess
{
    struct Square
    {
        int file = 0; // 0->a, 7->h
        int rank = 0; // 0->1, 7->8

        constexpr Square() = default;
        constexpr Square(int file, int rank)
            : file(file), rank(rank) {}

        constexpr inline bool operator==(const Square &other) const
        {
            return (file == other.file) && (rank == other.rank);
        }

        constexpr void operator+=(Offset offset)
        {
            file += offset.file;
            rank += offset.rank;
        }

        constexpr void operator-=(Offset offset)
        {
            file -= offset.file;
            rank -= offset.rank;
        }

        constexpr Square operator+(Offset offset) const
        {
            return Square{file + offset.file, rank + offset.rank};
        }

        constexpr Square operator-(Offset offset) const
        {
            return (*this) + (-offset);
        }

        constexpr Offset operator-(Square other) const
        {
            return Offset{file - other.file, rank - other.rank};
        }
    };
} // namespace ChessGame

template <>
struct std::hash<JChess::Square>
{
    inline size_t operator()(const JChess::Square &square) const noexcept
    {
        size_t h1 = std::hash<int>{}(square.file);
        size_t h2 = std::hash<int>{}(square.rank);
        return (h1 << 4) ^ h2;
    }
};
