#pragma once

#include <array>
#include <stdexcept>
#include <string>

#include "internal/logic/piece.h"

namespace ChessGame
{
    enum class CastleSide
    {
        QUEEN,
        KING,
    };

    constexpr inline uint8_t rookFromFile(const CastleSide side)
    {
        // 0 -> 0
        // 1 -> 7
        return static_cast<uint8_t>(7 * static_cast<int>(side));
    }

    constexpr inline uint8_t rookToFile(const CastleSide side)
    {
        // 0 -> 3
        // 1 -> 5
        return static_cast<uint8_t>(2 * static_cast<int>(side) + 3);
    }

    constexpr inline uint8_t kingToFile(const CastleSide side)
    {
        // 0 -> 2
        // 1 -> 6
        return static_cast<uint8_t>(4 * static_cast<int>(side) + 2);
    }

    struct Castle
    {
        Color color;
        CastleSide side;
    };

    class CastleRights
    {
    public:
        CastleRights() {}
        explicit CastleRights(std::string_view fenStringRights)
        {
            m_rights.fill(false);
            bool bad = false;
            for (const auto &c : fenStringRights)
            {
                int i = 0;
                switch (c)
                {
                case 'K':
                    i = 0;
                    break;
                case 'Q':
                    i = 1;
                    break;
                case 'k':
                    i = 2;
                    break;
                case 'q':
                    i = 3;
                    break;

                default:
                    bad = true;
                }
                if (m_rights[i])
                    bad = true;
                if (bad)
                    throw std::runtime_error("Invalid FEN castle string");
                m_rights[i] = true;
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
        constexpr void remove(Color color, CastleSide side)
        {
            m_rights[idx(color, side)] = false;
        }
        constexpr bool get(Color color, CastleSide side) const
        {
            return m_rights[idx(color, side)];
        }
        constexpr std::string str() const
        {
            std::string_view r{"KQkq"};
            std::string out{""};
            for (size_t i = 0; i < 4; ++i)
                if (!m_rights[i])
                    out += r[i];

            return out.empty() ? "-" : out;
        }

    private:
        constexpr inline int idx(Color color, CastleSide side) const
        {
            return 2 * static_cast<int>(color) + static_cast<int>(side);
        }
        std::array<bool, 4> m_rights = {true, true, true, true};
    };
}