#pragma once

#include <cstdint>
#include <optional>

#include "internal/logic/attacks.h"
#include "internal/logic/board.h"
#include "internal/logic/castling.h"
#include "internal/logic/move.h"
#include "internal/logic/piece.h"

namespace ChessGame
{
    constexpr std::string_view initFENFull{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

    /// @brief The game state of a game of chess
    /// @param fenString
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
        /// @brief Applies a move to the board state
        /// @param move
        void applyMove(const Move &move);
    };
} // namespace ChessGame
