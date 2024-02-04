#pragma once

#include <cstdint>
#include <optional>
#include <string>
#include <string_view>

#include "core/attacks.h"
#include "core/board.h"
#include "core/castling.h"
#include "core/color.h"
#include "core/move.h"
#include "core/square.h"

#include "formats/fen.h"

namespace JChess
{
    /// @brief The game state of a game of chess
    class State
    {
    public:
        Board board;
        uint32_t fullTurnCounter;
        uint32_t halfTurnCounter;
        Color turn;
        Castling::Rights castleRights;
        std::optional<Square> enPassant;
        Attacks attacks;

    public:
        State();
        State(std::string_view fenstr = FEN::startstate);
        /// @brief Applies a move to the board state
        /// @param move
        void applyMove(const Move &move);

        std::string toFEN() const;
    };
} // namespace JChess
