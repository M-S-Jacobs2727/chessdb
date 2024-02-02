#include "core/castling.h"

#include <stdexcept>

namespace ChessGame::Castling
{
    Rights::Rights(std::string_view fenStringRights)
    {
        reset(false);
        if (fenStringRights == "-")
            return;
        for (const auto &c : fenStringRights)
        {
            auto it = m_fenMap.find(c);
            if (it == m_fenMap.end())
                throw std::runtime_error("Invalid FEN castle string");
            m_rights[(*it).second] = true;
        }
    }

    constexpr void Rights::reset(bool val = true)
    {
        m_rights.fill(val);
    }

    constexpr void Rights::remove(Color color, Side side)
    {
        m_rights[idx(color, side)] = false;
    }

    constexpr void Rights::remove(Color color)
    {
        m_rights[idx(color, Side::QUEEN)] = false;
        m_rights[idx(color, Side::KING)] = false;
    }

    constexpr bool Rights::get(Color color, Side side) const
    {
        return m_rights[idx(color, side)];
    }

    constexpr const std::array<bool, 4> &Rights::get() const
    {
        return m_rights;
    }

    constexpr int Rights::idx(Color color, Side side) const
    {
        return 2 * static_cast<int>(color) - static_cast<int>(side) + 1;
    }

} // namespace ChessGame::Castling
