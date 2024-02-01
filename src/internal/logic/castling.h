#pragma once

#include <array>
#include <stdexcept>
#include <string>
#include <unordered_map>

#include "internal/logic/piece.h"
#include "internal/logic/square.h"

namespace ChessGame
{
    namespace Castling
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

        constexpr uint32_t rookFromFile(const Side side)
        {
            // QS/0 -> 0
            // KS/1 -> 7
            return 7u * static_cast<uint32_t>(side);
        }

        constexpr uint32_t rookToFile(const Side side)
        {
            // QS/0 -> 3
            // KS/1 -> 5
            return 2u * static_cast<uint32_t>(side) + 3u;
        }

        constexpr uint32_t kingToFile(const Side side)
        {
            // QS/0 -> 2
            // KS/1 -> 6
            return 4u * static_cast<uint32_t>(side) + 2u;
        }

        constexpr Square rookFromSquare(const Color color, const Side side)
        {
            return Square{rookFromFile(side), homeRank(color)};
        }

        constexpr Square rookToSquare(const Color color, const Side side)
        {
            return Square{rookToFile(side), homeRank(color)};
        }

        constexpr Square kingToSquare(const Color color, const Side side)
        {
            return Square{kingToFile(side), homeRank(color)};
        }
    }
}