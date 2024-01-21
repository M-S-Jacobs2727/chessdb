#pragma once

#include <unordered_set>
#include <utility>
#include <vector>

#include "internal/logic/attackedSquares.h"
#include "internal/logic/move.h"
#include "internal/logic/offset.h"
#include "internal/logic/square.h"
#include "internal/logic/state.h"

namespace ChessGame
{
    std::unordered_set<Move> legalMoves(const State &state, const AttackedSquares &attacked);
    std::unordered_set<Move> candidateMoves(const Position &pos, Square square);
    std::unordered_set<Move> candidatePawnMoves(const Position &pos, Square square, Color color);
    std::unordered_set<Move> candidateKnightMoves(const Position &pos, Square square, Color color);
    std::unordered_set<Move> candidateBishopMoves(const Position &pos, Square square, Color color);
    std::unordered_set<Move> candidateRookMoves(const Position &pos, Square square, Color color);
    std::unordered_set<Move> candidateQueenMoves(const Position &pos, Square square, Color color);
    std::unordered_set<Move> candidateKingMoves(const Position &pos, Square square, Color color);
    std::optional<std::pair<Square, Offset>> getHardPin(const Position &pos, Square square, Color color);
    std::optional<Square> searchAlongPath(const Position &pos, Square fromSquare, Offset direction);
} // namespace ChessGame