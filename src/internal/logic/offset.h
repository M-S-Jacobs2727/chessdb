#pragma once

#include <array>
#include <optional>

#include "internal/logic/piece.h"
#include "internal/logic/square.h"

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

        constexpr std::optional<Square> operator()(const Square &square) const
        {
            Square s{static_cast<uint8_t>(static_cast<int>(square.file) + file),
                     static_cast<uint8_t>(static_cast<int>(square.rank) + rank)};
            return s.valid() ? std::make_optional(s) : std::nullopt;
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
                (*this) /= MAX(abs(file), abs(rank));
                return true;
            }

            if (isDiagonal())
            {
                (*this) /= abs(file);
                return true;
            }

            return false;
        }
    };

    constexpr Offset difference(Square from, Square to)
    {
        return Offset{to.file - from.file, to.rank - from.rank};
    }

    constexpr inline Offset forward(Color color)
    {
        return Offset{0, static_cast<int>(color) * -2 + 1};
    }

    constexpr inline Offset backward(Color color)
    {
        return Offset{0, static_cast<int>(color) * 2 - 1};
    }

    const inline std::array<Offset, 4> pawnOffsets{{{0, 1},
                                                    {0, 2},
                                                    {-1, 1},
                                                    {1, 1}}};
    const inline std::array<Offset, 2> pawnAttackOffsets{{{-1, 1},
                                                          {1, 1}}};
    const inline std::array<Offset, 8> knightOffsets{{{-2, -1},
                                                      {-1, -2},
                                                      {1, -2},
                                                      {2, -1},
                                                      {2, 1},
                                                      {1, 2},
                                                      {-1, 2},
                                                      {-2, 1}}};
    const inline std::array<Offset, 4> bishopOffsets{{{-1, -1},
                                                      {-1, 1},
                                                      {1, -1},
                                                      {1, 1}}};
    const inline std::array<Offset, 4> rookOffsets{{{-1, 0},
                                                    {1, 0},
                                                    {0, -1},
                                                    {0, 1}}};
    const inline std::array<Offset, 8> queenKingOffsets{{{-1, -1},
                                                         {-1, 1},
                                                         {1, -1},
                                                         {1, 1},
                                                         {-1, 0},
                                                         {1, 0},
                                                         {0, -1},
                                                         {0, 1}}};
}