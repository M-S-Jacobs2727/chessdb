#pragma once

#include <cstdint>
#include <string_view>

namespace ChessGame
{
    struct Square
    {
        uint8_t file = 0; // 0-7, other values invalid
        uint8_t rank = 0; // 0-7, reversed from Position::idx()
        constexpr Square() = default;
        constexpr Square(uint8_t file, uint8_t rank) : file(file), rank(rank) {}
        explicit Square(const std::string_view &str) : file(str[0] - 'a'), rank(str[1] - '1') {}
        std::string_view str() const
        {
            char s[3] = "a1";
            s[0] += file;
            s[1] += rank;
            return std::string_view(s);
        }
        bool valid() const
        {
            return file <= 7 && rank <= 7;
        }

        bool operator==(const Square &other) const = default;
    };
} // namespace ChessGame
