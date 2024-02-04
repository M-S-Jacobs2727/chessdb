#pragma once

#include <array>
#include <string>
#include <string_view>
#include <unordered_map>

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
        explicit Rights(std::string_view fenStringRights = FEN::startcastle);
        constexpr void reset(bool val = true);
        constexpr void remove(Color color, Side side);
        constexpr void remove(Color color);
        constexpr bool get(Color color, Side side) const;
        constexpr const std::array<bool, 4> &get() const;

        constexpr std::string toFEN() const;

    private:
        constexpr inline int idx(Color color, Side side) const;
        std::array<bool, 4> m_rights = {true, true, true, true};
    };
} // namespace JChess::Castling