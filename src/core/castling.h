#pragma once

#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

#include "core/color.h"
#include "core/square.h"

namespace ChessGame::Castling
{
    enum class Side
    {
        QUEEN,
        KING,
    };

    class Rights
    {
    public:
        Rights() = default;
        explicit Rights(std::string_view fenStringRights);
        constexpr void reset(bool val = true);
        constexpr void remove(Color color, Side side);
        constexpr void remove(Color color);
        constexpr bool get(Color color, Side side) const;
        constexpr std::string str() const;

    private:
        constexpr inline int idx(Color color, Side side) const;
        std::array<bool, 4> m_rights = {true, true, true, true};
        std::unordered_map<char, size_t> m_fenMap = {
            {'K', 0},
            {'Q', 1},
            {'k', 2},
            {'q', 3},
        };
    };
} // namespace ChessGame::Castling