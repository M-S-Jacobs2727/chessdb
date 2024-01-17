#pragma once

#include <array>
#include <memory>
#include <unordered_set>

#include "internal/logic/move.h"
#include "internal/logic/piece.h"
#include "internal/logic/position.h"
#include "internal/logic/square.h"

namespace ChessGame
{
    class AttackedSquares
    {
    public:
        AttackedSquares(Color color, std::shared_ptr<Position> pos);

        // To be called after Position::applyMove
        // Assumes that the referenced position is the new state
        void applyMove(const Move &move);

        // Compute the attacking pieces from scratch
        // Should only need to be called at construction, may make private in the future
        void recompute();

        // Retrieve a read-only view of the set of squares attacking the given square
        const std::unordered_set<Square> &attackers(Square sq) const;

        // Ascertain whether the given square is attacked
        bool attacked(Square sq) const;

    private:
        uint8_t idx(Square square) const;
        Square idxToSquare(uint8_t idx) const;
        void addAttacker(Square sq, PieceType pt);
        void removeAttacker(Square sq, PieceType pt);
        void addPiece(Square sq);
        void removePiece(Square sq);
        std::shared_ptr<Position> getPos() const;

    private:
        // Color of the attacking pieces
        Color m_color;

        std::weak_ptr<Position> m_pos;

        // for each square, the set of squares containing pieces of color m_color that attack this square
        std::array<std::unordered_set<Square>, 64> m_squares;
    };
} // namespace ChessGame