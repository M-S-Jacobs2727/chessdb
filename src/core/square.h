#pragma once

#include <cstdint>
#include <functional>
#include <stdexcept>
#include <string>
#include <string_view>

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
        constexpr Square(std::string_view str)
        {
            auto len = str.size();
            if (len < 2 || len > 3)
                throw std::runtime_error("Invalid string length for Square");

            char f = str[0];
            if (f < 'a' || 'z' < f)
                throw std::runtime_error("Invalid file value for Square from string");
            file = static_cast<int>(f - 'a');

            rank = std::stol(str.substr(1).data()) - 1;
        }

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
} // namespace JChess
