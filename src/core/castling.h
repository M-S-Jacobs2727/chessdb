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
        explicit Rights(std::string_view fenStringRights = FEN::startcastle);
        void reset(bool val = true);
        void remove(Color color, Side side);
        void remove(Color color);
        bool get(Color color, Side side) const;
        const std::array<bool, 4> &get() const;

        std::string toFEN() const;

    private:
        int idx(Color color, Side side) const;
        std::array<bool, 4> m_rights = {true, true, true, true};
    };
} // namespace JChess::Castling