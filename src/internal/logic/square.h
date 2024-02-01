#pragma once

#include <cstdint>
#include <functional>

namespace ChessGame
{
    struct Square
    {
        uint32_t file = 0; // 0->a, 7->h
        uint32_t rank = 0; // 0->1, 7->8

        constexpr Square() = default;
        constexpr Square(uint32_t file, uint32_t rank)
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
        size_t h1 = std::hash<size_t>{}(static_cast<size_t>(square.file));
        size_t h2 = std::hash<size_t>{}(static_cast<size_t>(square.rank));
        return (h1 << 1) ^ h2;
    }
};
