#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <vector>

#include "core/board.h"
#include "core/color.h"
#include "core/move.h"
#include "core/piece.h"
#include "core/square.h"

namespace ChessGame
{
    using AttackerArray = std::array<std::bitset<64>, 64>;

    // TODO: Rename everything here
    class Attacks
    {
    public:
        Attacks() = delete;
        explicit Attacks(std::shared_ptr<Board> board);

        /* The squares that the piece on the given square attacks.
         */
        std::vector<Square> attacks(Square square) const;

        /* Retrieve a vector of squares occupied by pieces of the given color attacking
        the given square.
         */
        std::vector<Square> attackers(Square square, Color color) const;

        /* Ascertain whether the given square is attacked by pieces of the given color.
         */
        bool attacked(Square square, Color color) const;

        /* Ascertain the number of attackers of a given color on the given square.
         */
        size_t numAttackers(Square square, Color color) const;

        /* To be called within Position::applyMove (at the end). Assumes that the
        referenced board is the new state.
         * Note: I would like to restrict access to this method exclusively to
         * `Position::applyMove` if possible, e.g., via friend specification, but I can't
         * figure it out. But it's also useful for testing.
         */
        void applyMove(const Move &move);

    private:
        // Compute the attacking pieces from scratch
        // Should only need to be called at construction, may make private in the future
        void recompute();

        void addAttacker(Square square, Piece piece);
        void removeAttacker(Square square, Piece piece);
        void addPiece(Square square);
        void removePiece(Square square);
        std::shared_ptr<Board> getBoard() const;

    private:
        // board pointer
        std::weak_ptr<Board> m_board_ptr;

        // Squares occupied by white pieces that attack the given square
        AttackerArray m_attackedByWhite;
        // Squares occupied by black pieces that attack the given square
        AttackerArray m_attackedByBlack;
        // Squares that the piece occupying the given square attacks
        AttackerArray m_attackedFrom;
    };
} // namespace ChessGame