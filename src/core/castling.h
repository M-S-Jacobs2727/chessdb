#pragma once

#include <array>
#include <string>
#include <string_view>

#include "core/color.h"
#include "core/square.h"

#include "formats/fen.h"

namespace JChess::Castling
{
    enum class Side
    {
        QUEEN,
        KING,
    };

    class Rights
    {
    public:
        explicit Rights(std::string_view fenStringRights = FEN::startcastle)
        {
            reset(false);
            if (fenStringRights == "-")
                return;
            for (const auto &c : fenStringRights)
                m_rights[FEN::charToCastleIdx(c)] = true;
        }
        constexpr void reset(bool val = true)
        {
            m_rights.fill(val);
        }
        constexpr void remove(Color color, Side side)
        {
            m_rights[idx(color, side)] = false;
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
        constexpr const std::array<bool, 4> &get() const
        {
            return m_rights;
        }

        std::string toFEN() const
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

    private:
        constexpr int idx(Color color, Side side) const
        {
            return 2 * static_cast<int>(color) - static_cast<int>(side) + 1;
        }
        std::array<bool, 4> m_rights = {true, true, true, true};
    };
} // namespace JChess::Castling