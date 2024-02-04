#include "core/castling.h"

#include <stdexcept>

namespace JChess::Castling
{
    Rights::Rights(std::string_view fenStringRights)
    {
        reset(false);
        if (fenStringRights == "-")
            return;
        for (const auto &c : fenStringRights)
            m_rights[FEN::charToCastleIdx(c)] = true;
    }

    constexpr void Rights::reset(bool val)
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

    constexpr std::string Rights::toFEN() const
    {
        std::string out;
        out.reserve(5);
        std::string_view test{"KQkq"};

        for (size_t i = 0; i < 4; ++i)
            if (m_rights[i])
                out += test[i];

        if (out.empty())
            out += '-';

        return out;
    }
} // namespace JChess::Castling
