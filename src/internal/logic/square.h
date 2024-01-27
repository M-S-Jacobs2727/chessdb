#pragma once

#include <cstdint>
#include <stdexcept>
#include <string_view>

namespace ChessGame
{
    struct Square
    {
        uint8_t file = 0; // 0-7, other values invalid
        uint8_t rank = 0; // 0-7, reversed from Position::idx()

        constexpr Square() = default;
        constexpr Square(uint8_t file, uint8_t rank)
            : file(file), rank(rank)
        {
            if (!valid())
                throw std::runtime_error("Invalid Square");
        }
        explicit Square(const std::string_view &str)
            : file(str[0] - 'a'), rank(str[1] - '1')
        {
            if (!valid())
                throw std::runtime_error("Invalid Square");
        }

        constexpr static Square fromIdx(uint32_t idx)
        {
            if (idx >= 64)
                throw std::runtime_error("Invalid idx");
            uint32_t q = idx / 8, r = idx % 8;
            return Square{r, 7 - q};
        }

        constexpr inline bool operator==(const Square &other) const
        {
            return (file == other.file) && (rank == other.rank);
        }
        std::string_view str() const
        {
            char s[3] = "a1";
            s[0] += file;
            s[1] += rank;
            return std::string_view(s);
        }
        constexpr inline bool valid() const
        {
            return file <= 7 && rank <= 7;
        }
        constexpr inline uint8_t idx() const
        {
            return (7 - rank) * 8 + file;
        }
    };

    constexpr inline uint8_t homeRank(const Color color)
    {
        return -7 * (static_cast<int>(color) - 1);
    }
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
