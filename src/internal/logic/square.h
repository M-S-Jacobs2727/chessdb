#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>
#include <string_view>

namespace ChessGame
{
    struct Square
    {
        uint8_t file = 0; // 0->a, 7->h
        uint8_t rank = 0; // 0->1, 7->8

        constexpr Square() = default;
        constexpr Square(uint8_t file, uint8_t rank)
            : file(file), rank(rank) {}
        explicit Square(const std::string_view &str)
            : file(str[0] - 'a'), rank(str[1] - '1') {}

        constexpr inline bool operator==(const Square &other) const
        {
            return (file == other.file) && (rank == other.rank);
        }
        std::string str() const
        {
            std::string s{"a1"};
            s[0] += file; // 0->a, 7->h
            s[1] += rank; // 0->1, 7->8
            return s;
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
