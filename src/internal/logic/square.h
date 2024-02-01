#pragma once

#include <cstdint>
#include <functional>

namespace ChessGame
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
    };

} // namespace ChessGame

template <>
struct std::hash<ChessGame::Square>
{
    inline size_t operator()(const ChessGame::Square &square) const noexcept
    {
        size_t h1 = std::hash<int>{}(square.file);
        size_t h2 = std::hash<int>{}(square.rank);
        return (h1 << 4) ^ h2;
    }
};
