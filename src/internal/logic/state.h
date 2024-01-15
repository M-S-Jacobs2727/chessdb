#pragma once

#include <array>
#include <cstdint>
#include <optional>

#include "internal/logic/castleRights.h"
#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/position.h"

namespace ChessGame
{
    class State
    {
    public:
        explicit State(std::string_view fenString = INITFEN);
        void applyMove(const Move &move);
        Move applyPGNMove(std::string_view pgnMove);
        Move applyUCIMove(std::string_view uciMove);

    public:
        Position position;
        uint8_t fullTurnCounter;
        uint8_t halfTurnCounter;
        Color turn = Color::White;
        CastleRights castleRights;
        std::optional<Square> enPassant;

    private:
        void determineFromSquare(Move &move);
        void determineExtra(const Move &move);
        uint8_t homeRank(Color color);
    };
} // namespace ChessGame
