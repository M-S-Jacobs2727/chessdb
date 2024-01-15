#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/position.h"

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

    class State
    {
    public:
        explicit State(std::string_view fenString = INITFEN);
        void applyMove(Move &move);

    public:
        Position position;
        uint8_t fullTurnCounter = 1;
        uint8_t halfTurnCounter = 0;
        Color turn = Color::White;
        CastleRights castleRights;
        std::optional<Square> enPassant = std::nullopt;

    private:
        void determineFromSquare(Move &move);
        void determineExtra(const Move &move);
        uint8_t homeRank(Color color);
    };
} // namespace ChessGame
