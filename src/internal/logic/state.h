#pragma once

#include <cstdint>
#include <optional>

#include "internal/logic/castling.h"
#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/position.h"

namespace ChessGame
{
    class State
    {
    public:
        Position position;
        uint8_t fullTurnCounter;
        uint8_t halfTurnCounter;
        Color turn;
        CastleRights castleRights;
        std::optional<Square> enPassant;

    public:
        explicit State(std::string_view fenString = INITFEN);
        void applyMove(const Move &move);
        Move applyPGNMove(std::string_view pgnMove);
        Move applyUCIMove(std::string_view uciMove);
    };

    Square computeFromSquare(Square to,
                             Piece piece,
                             const Position &pos,
                             std::string_view partialFrom);
    Move interpretPGNMove(std::string_view pgnMove,
                          const Position &pos,
                          Color turn);
    Move interpretUCIMove(std::string_view uciMove, const State &state);

} // namespace ChessGame
