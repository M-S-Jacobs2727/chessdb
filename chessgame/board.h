#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "move.h"
#include "piece.h"
#include "pieces.h"

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

    private:
        constexpr inline int idx(Color color, Side side) const
        {
            return 2 * static_cast<int>(color) + static_cast<int>(side);
        }
        std::array<bool, 4> m_rights = {true, true, true, true};
    };

    class Board
    {
    public:
        explicit Board(std::string_view fenString = INITFEN);
        void applyMove(Move &move);

    public:
        Pieces pieces;
        uint8_t fullTurnCounter = 1;
        uint8_t halfTurnCounter = 0;
        Color turn = Color::White;
        CastleRights rights;
        std::optional<Square> enPassant = std::nullopt;

    private:
        void determineFromCapture(Move &move);
        void determineExtra(Move &move);
        uint8_t homeRank(Color color);
    };
} // namespace ChessGame
