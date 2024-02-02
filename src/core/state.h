#pragma once

#include <cstdint>
#include <memory>
#include <optional>

#include "core/attacks.h"
#include "core/board.h"
#include "core/castling.h"
#include "core/color.h"
#include "core/move.h"
#include "core/square.h"

namespace ChessGame
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
        State() : board(initFEN), attacks(std::make_shared<Board>(board)) {}
        /// @brief Applies a move to the board state
        /// @param move
        void applyMove(const Move &move);
    };
} // namespace ChessGame
