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

        struct Castle
        {
            Color color;
            Side side;
        };

        constexpr Square rookFromSquare(const Castle castle)
        {
            return Square{rookFromFile(castle.side), homeRank(castle.color)};
        }

        constexpr Square rookToSquare(const Castle castle)
        {
            return Square{rookToFile(castle.side), homeRank(castle.color)};
        }

        constexpr Square kingToSquare(const Castle castle)
        {
            return Square{kingToFile(castle.side), homeRank(castle.color)};
        }

        class Rights
        {
        public:
            Rights() = default;
            explicit Rights(std::string_view fenStringRights)
            {
                reset(false);
                if (fenStringRights == "-")
                    return;
                for (const auto &c : fenStringRights)
                {
                    if (m_fenMap.find(c) == m_fenMap.end())
                        throw std::runtime_error("Invalid FEN castle string");
                    m_rights[m_fenMap.at(c)] = true;
                }
            }
            // constexpr void set(Color color, Side side, bool val = true)
            // {
            //     m_rights[idx(color, side)] = val;
            // }
            constexpr void reset(bool val = true)
            {
                m_rights = {val, val, val, val};
            }
            constexpr void remove(Castle castle)
            {
                m_rights[idx(castle.color, castle.side)] = false;
            }
            constexpr void remove(Color color)
            {
                m_rights[idx(color, Side::QUEEN)] = false;
                m_rights[idx(color, Side::KING)] = false;
            }
            constexpr bool get(Color color, Side side) const
            {
                return m_rights[idx(color, side)];
            }
            constexpr std::string str() const
            {
                std::string_view r{"KQkq"};
                std::string out{""};
                for (size_t i = 0; i < 4; ++i)
                    if (m_rights[i])
                        out += r[i];

                if (out.empty())
                    out += '-';

                return out;
            }

        private:
            constexpr inline int idx(Color color, Side side) const
            {
                return 2 * static_cast<int>(color) + static_cast<int>(side);
            }
            std::array<bool, 4> m_rights = {true, true, true, true};
            std::unordered_map<char, size_t> m_fenMap = {
                {'K', 0},
                {'Q', 1},
                {'k', 2},
                {'1', 3},
            };
        };
    }
}