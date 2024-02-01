#pragma once

#include <array>
#include <cstdlib>

#include "internal/logic/color.h"

#define MAX(x, y) (x < y) ? y : x

namespace ChessGame
{
    struct Offset
    {
        int file = 0;
        int rank = 0;

        constexpr bool operator==(const Offset &other) const
        {
            return (file == other.file) && (rank == other.rank);
        }

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

        constexpr bool isLateral() const
        {
            return file == 0 || rank == 0;
        }

        constexpr bool isDiagonal() const
        {
            return abs(file) == abs(rank);
        }

        bool norm()
        {
            if (isLateral())
            {
                auto val = MAX(abs(file), abs(rank));
                file /= val;
                rank /= val;
                return true;
            }

            if (isDiagonal())
            {
                auto val = abs(file);
                file /= val;
                rank /= val;
                return true;
            }

            return false;
        }
    };

    namespace Offsets
    {
        constexpr inline Offset forward(Color color)
        {
            return Offset{0, static_cast<int>(color) * -2 + 1};
        }

        constexpr inline Offset backward(Color color)
        {
            return Offset{0, static_cast<int>(color) * 2 - 1};
        }

        const inline std::array<Offset, 4> pawn{{{0, 1},
                                                 {0, 2},
                                                 {-1, 1},
                                                 {1, 1}}};
        const inline std::array<Offset, 2> pawnAttack{{{-1, 1},
                                                       {1, 1}}};
        const inline std::array<Offset, 8> knight{{{-2, -1},
                                                   {-1, -2},
                                                   {1, -2},
                                                   {2, -1},
                                                   {2, 1},
                                                   {1, 2},
                                                   {-1, 2},
                                                   {-2, 1}}};
        const inline std::array<Offset, 4> bishop{{{-1, -1},
                                                   {-1, 1},
                                                   {1, -1},
                                                   {1, 1}}};
        const inline std::array<Offset, 4> rook{{{-1, 0},
                                                 {1, 0},
                                                 {0, -1},
                                                 {0, 1}}};
        const inline std::array<Offset, 8> queenKing{{{-1, -1},
                                                      {-1, 1},
                                                      {1, -1},
                                                      {1, 1},
                                                      {-1, 0},
                                                      {1, 0},
                                                      {0, -1},
                                                      {0, 1}}};
    }
}