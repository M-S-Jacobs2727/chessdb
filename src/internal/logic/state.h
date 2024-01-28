#pragma once

#include <cstdint>
#include <optional>

#include "internal/logic/attacks.h"
#include "internal/logic/castling.h"
#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/position.h"

namespace ChessGame
{
    constexpr std::string_view initFENFull{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"};

    /// @brief The game state of a game of chess
    /// @param fenString
    class State
    {
    public:
        Position position;
        uint32_t fullTurnCounter;
        uint32_t halfTurnCounter;
        Color turn;
        Castling::Rights castleRights;
        std::optional<Square> enPassant;

    private:
        std::shared_ptr<Position> m_pos;

    public:
        Attacks attacks;

    public:
        explicit State(std::string_view fenString = initFENFull);

        /// @brief Applies a move to the board state
        /// @param move
        void applyMove(const Move &move);

        /// @brief Applies a PGN-formatted move to the board state
        /// @param pgnMove The PGN-formatted move
        /// @return A full description of the move applied
        Move applyPGNMove(std::string_view pgnMove);

        /// @brief Applies a move formatted in long algebraic notation to the board state
        /// @param pgnMove The move formatted in long algebraic notation
        /// @return A full description of the move applied
        Move applyUCIMove(std::string_view uciMove);
    };
} // namespace ChessGame
