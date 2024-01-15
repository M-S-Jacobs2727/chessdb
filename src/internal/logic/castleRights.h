#pragma once

#include <array>
#include <string>

#include "internal/logic/piece.h"

namespace ChessGame
{
    class CastleRights
    {
    public:
        enum class Side
        {
            KING,
            QUEEN,
        };
        constexpr void set(Color color, Side side, bool val = true)
        {
            m_rights[idx(color, side)] = val;
        }
        constexpr void reset(bool val = true)
        {
            m_rights = {val, val, val, val};
        }
        constexpr void remove(Color color, Side side)
        {
            m_rights[idx(color, side)] = false;
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
                if (!m_rights[i])
                    out += r[i];

            return out.empty() ? "-" : out;
        }

    private:
        constexpr inline int idx(Color color, Side side) const
        {
            return 2 * static_cast<int>(color) + static_cast<int>(side);
        }
        std::array<bool, 4> m_rights = {true, true, true, true};
    };
}