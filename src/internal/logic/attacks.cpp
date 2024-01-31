#include <array>

#include "internal/logic/attacks.h"
#include "internal/logic/offset.h"

namespace ChessGame
{
    Attacks::Attacks(std::shared_ptr<Position> pos)
        : m_pos(pos)
    {
        recompute();
    }

    void Attacks::recompute()
    {
        for (auto &bs : m_attackedByWhite)
            bs.reset();
        for (auto &bs : m_attackedByBlack)
            bs.reset();
        for (auto &bs : m_attackedFrom)
            bs.reset();

        auto pos = getPos();

        for (const auto &[sq, occupant] : pos->eachSquare())
            if (occupant)
                addAttacker(sq, occupant.value());
    }

    std::vector<Square> Attacks::attackers(Square square, Color color) const
    {
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[square.idx()] : m_attackedByBlack[square.idx()];
        std::vector<Square> attackers;
        attackers.reserve(attackedBy.count());

        for (size_t i = 0; i < 64; ++i)
            if (attackedBy[i])
                attackers.push_back(Square::fromIdx(i));

        return attackers;
    }

    bool Attacks::attacked(Square square, Color color) const
    {
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[square.idx()] : m_attackedByBlack[square.idx()];
        return attackedBy.any();
    }

    int Attacks::numAttackers(Square square, Color color) const
    {
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[square.idx()] : m_attackedByBlack[square.idx()];
        return attackedBy.count();
    }

    void Attacks::applyMove(const Move &move)
    {
        Color activeColor = move.piece.color;
        Color otherColor = oppositeColor(activeColor);

        removePiece(move.from);
        if (!move.capture || move.enPassant)
            addPiece(move.to);
        removeAttacker(move.from, move.piece);

        if (move.enPassant)
        {
            Square capturedPawnSq = backward(activeColor)(move.to).value();
            removePiece(capturedPawnSq);
            removeAttacker(capturedPawnSq, Piece{otherColor, PieceType::Pawn});
        }
        else if (move.castle)
        {
            bool qs = move.castle.value() == Castling::Side::QUEEN;
            Square rookTo = Square{qs ? 3u : 5u, move.from.rank},
                   rookFrom = Square{qs ? 0u : 7u, move.from.rank};
            removePiece(rookFrom);
            addPiece(rookTo);
            Piece rook{activeColor, PieceType::Rook};
            removeAttacker(rookFrom, rook);
            addAttacker(rookTo, rook);
        }
        else if (move.capture)
        {
            removeAttacker(move.to, Piece{otherColor, move.capture.value()});
        }

        Piece addedAttacker = Piece{activeColor, move.promotion.value_or(move.piece.type)};
        addAttacker(move.to, addedAttacker);
    }

    void Attacks::addAttacker(Square square, Piece piece)
    {
        std::optional<Square> maybeSq = std::nullopt;
        Square newSq{};
        auto pos = getPos();

        std::vector<Square> attackedSquares;
        attackedSquares.reserve(16);

        switch (piece.type)
        {
        case PieceType::Pawn:
            for (auto offset : pawnAttackOffsets)
            {
                if (piece.color == Color::White)
                    offset.rank *= -1;
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : knightOffsets)
            {
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : bishopOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Rook:
            for (const auto &offset : rookOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Queen:
            for (const auto &offset : queenKingOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::King:
            for (const auto &offset : queenKingOffsets)
            {
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        default:
            std::unreachable();
            break;
        }

        for (const auto &attacked : attackedSquares)
            m_attackedFrom[square.idx()][attacked.idx()] = 1;

        if (piece.color == Color::White)
            for (const auto &attacked : attackedSquares)
                m_attackedByWhite[attacked.idx()][square.idx()] = 1;
        else
            for (const auto &attacked : attackedSquares)
                m_attackedByBlack[attacked.idx()][square.idx()] = 1;
    }

    void Attacks::removeAttacker(Square square, Piece piece)
    {
        std::optional<Square> maybeSq = std::nullopt;
        Square newSq{};
        auto pos = getPos();

        std::vector<Square> attackedSquares;
        attackedSquares.reserve(16);

        switch (piece.type)
        {
        case PieceType::Pawn:
            for (auto offset : pawnAttackOffsets)
            {
                if (piece.color == Color::White)
                    offset.rank *= -1;
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : knightOffsets)
            {
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : bishopOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Rook:
            for (const auto &offset : rookOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Queen:
            for (const auto &offset : queenKingOffsets)
                for (const auto &newSq : pos->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::King:
            for (const auto &offset : queenKingOffsets)
            {
                maybeSq = offset(square);
                if (maybeSq)
                    attackedSquares.push_back(maybeSq.value());
            }
            break;

        default:
            std::unreachable();
        }

        for (const auto &attacked : attackedSquares)
            m_attackedFrom[square.idx()][attacked.idx()] = 0;

        if (piece.color == Color::White)
            for (const auto &attacked : attackedSquares)
                m_attackedByWhite[attacked.idx()][square.idx()] = 0;
        else
            for (const auto &attacked : attackedSquares)
                m_attackedByBlack[attacked.idx()][square.idx()] = 0;
    }

    void Attacks::addPiece(Square square)
    {
        auto pos = getPos();

        for (size_t i = 0; i < 64; ++i)
        {
            if (!m_attackedFrom[i][square.idx()])
                continue;
            Square attacker = Square::fromIdx(i);

            [[unlikely]] if (!pos->get(attacker))
                throw std::runtime_error("Invalid attacker");
            auto piece = pos->get(attacker).value();

            auto &attackedBy = (piece.color == Color::White) ? m_attackedByWhite : m_attackedByBlack;

            if (piece.type == PieceType::Pawn ||
                piece.type == PieceType::Knight ||
                piece.type == PieceType::King)
                continue;

            Offset offset{square.file - attacker.file,
                          square.rank - attacker.rank};
            offset.norm();

            for (const auto &newSq : pos->getPath(square, offset, true))
            {
                attackedBy[newSq.idx()][i] = 0;
                m_attackedFrom[i][newSq.idx()] = 0;
            }
        }
    }

    void Attacks::removePiece(Square square)
    {
        auto pos = getPos();

        for (size_t i = 0; i < 64; ++i)
        {
            if (!m_attackedFrom[i][square.idx()])
                continue;
            Square attacker = Square::fromIdx(i);

            [[unlikely]] if (!pos->get(attacker))
                throw std::runtime_error("Invalid attacker");
            auto piece = pos->get(attacker).value();

            auto &attackedBy = (piece.color == Color::White) ? m_attackedByWhite : m_attackedByBlack;

            if (piece.type == PieceType::Pawn ||
                piece.type == PieceType::Knight ||
                piece.type == PieceType::King)
                continue;

            Offset offset{square.file - attacker.file,
                          square.rank - attacker.rank};
            offset.norm();

            for (const auto &newSq : pos->getPath(square, offset, true))
            {
                attackedBy[newSq.idx()][i] = 1;
                m_attackedFrom[i][newSq.idx()] = 1;
            }
        }
    }

    std::shared_ptr<Position> Attacks::getPos() const
    {
        std::shared_ptr<Position> sp_pos = m_pos.lock();
        if (!sp_pos)
            throw std::runtime_error("AttackingPieces could not access Postion");
        return sp_pos;
    }
} // namespace ChessGame