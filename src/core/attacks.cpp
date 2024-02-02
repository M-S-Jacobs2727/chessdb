#include "core/attacks.h"

#include <ranges>
#include <utility>

#include "core/offset.h"

namespace ChessGame
{
    Attacks::Attacks(std::shared_ptr<Board> board)
        : m_board_ptr(board)
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

        auto board = getBoard();

        for (const auto &[square, occupant] : std::views::zip(board->eachSquare(), board->eachOccupant()))
            if (occupant)
                addAttacker(square, occupant.piece());
    }

    std::vector<Square> Attacks::attackers(Square square, Color color) const
    {
        auto board = getBoard();
        auto idx = board->squareToIdx(square);
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[idx]
                                                   : m_attackedByBlack[idx];
        std::vector<Square> attackers;
        attackers.reserve(attackedBy.count());

        for (size_t i = 0; i < 64; ++i)
            if (attackedBy[i])
                attackers.push_back(board->idxToSquare(i));

        return attackers;
    }

    bool Attacks::attacked(Square square, Color color) const
    {
        auto board = getBoard();
        auto idx = board->squareToIdx(square);
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[idx]
                                                   : m_attackedByBlack[idx];
        return attackedBy.any();
    }

    size_t Attacks::numAttackers(Square square, Color color) const
    {
        auto board = getBoard();
        auto idx = board->squareToIdx(square);
        auto &attackedBy = (color == Color::White) ? m_attackedByWhite[idx]
                                                   : m_attackedByBlack[idx];
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
            Square capturedPawnSq = move.to + Offsets::backward(activeColor);
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
        Square newSq{};
        auto board = getBoard();

        std::vector<Square> attackedSquares;
        attackedSquares.reserve(16);

        switch (piece.type)
        {
        case PieceType::Pawn:
            for (auto offset : Offsets::pawnAttack)
            {
                if (piece.color == Color::Black)
                    offset.rank *= -1;
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : Offsets::knight)
            {
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : Offsets::bishop)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Rook:
            for (const auto &offset : Offsets::rook)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Queen:
            for (const auto &offset : Offsets::queenKing)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::King:
            for (const auto &offset : Offsets::queenKing)
            {
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        default:
            std::unreachable();
            break;
        }

        auto idx = board->squareToIdx(square);
        for (const auto &attacked : attackedSquares)
            m_attackedFrom[idx][board->squareToIdx(attacked)] = 1;

        if (piece.color == Color::White)
            for (const auto &attacked : attackedSquares)
                m_attackedByWhite[board->squareToIdx(attacked)][idx] = 1;
        else
            for (const auto &attacked : attackedSquares)
                m_attackedByBlack[board->squareToIdx(attacked)][idx] = 1;
    }

    void Attacks::removeAttacker(Square square, Piece piece)
    {
        std::optional<Square> maybeSq = std::nullopt;
        Square newSq{};
        auto board = getBoard();

        std::vector<Square> attackedSquares;
        attackedSquares.reserve(16);

        switch (piece.type)
        {
        case PieceType::Pawn:
            for (auto offset : Offsets::pawnAttack)
            {
                if (piece.color == Color::White)
                    offset.rank *= -1;
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : Offsets::knight)
            {
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : Offsets::bishop)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Rook:
            for (const auto &offset : Offsets::rook)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::Queen:
            for (const auto &offset : Offsets::queenKing)
                for (const auto &newSq : board->getPath(square, offset, true))
                    attackedSquares.push_back(newSq);
            break;

        case PieceType::King:
            for (const auto &offset : Offsets::queenKing)
            {
                newSq = square + offset;
                if (board->valid(newSq))
                    attackedSquares.push_back(newSq);
            }
            break;

        default:
            std::unreachable();
        }

        auto idx = board->squareToIdx(square);
        for (const auto &attacked : attackedSquares)
            m_attackedFrom[idx][board->squareToIdx(attacked)] = 0;

        if (piece.color == Color::White)
            for (const auto &attacked : attackedSquares)
                m_attackedByWhite[board->squareToIdx(attacked)][idx] = 0;
        else
            for (const auto &attacked : attackedSquares)
                m_attackedByBlack[board->squareToIdx(attacked)][idx] = 0;
    }

    void Attacks::addPiece(Square square)
    {
        auto board = getBoard();

        for (size_t i = 0; i < 64; ++i)
        {
            if (!m_attackedFrom[i][board->squareToIdx(square)])
                continue;
            Square attacker = board->idxToSquare(i);

            [[unlikely]] if (!board->get(attacker))
                throw std::runtime_error("Invalid attacker");
            auto piece = board->get(attacker).piece();

            auto &attackedBy = (piece.color == Color::White) ? m_attackedByWhite : m_attackedByBlack;

            if (piece.type == PieceType::Pawn ||
                piece.type == PieceType::Knight ||
                piece.type == PieceType::King)
                continue;

            Offset offset{square.file - attacker.file,
                          square.rank - attacker.rank};
            offset.norm();

            for (const auto &newSq : board->getPath(square, offset, true))
            {
                auto idx = board->squareToIdx(newSq);
                attackedBy[idx][i] = 0;
                m_attackedFrom[i][idx] = 0;
            }
        }
    }

    void Attacks::removePiece(Square square)
    {
        auto board = getBoard();

        for (size_t i = 0; i < 64; ++i)
        {
            if (!m_attackedFrom[i][board->squareToIdx(square)])
                continue;
            Square attacker = board->idxToSquare(i);

            [[unlikely]] if (!board->get(attacker))
                throw std::runtime_error("Invalid attacker");
            auto piece = board->get(attacker).piece();

            auto &attackedBy = (piece.color == Color::White) ? m_attackedByWhite : m_attackedByBlack;

            if (piece.type == PieceType::Pawn ||
                piece.type == PieceType::Knight ||
                piece.type == PieceType::King)
                continue;

            Offset offset{square.file - attacker.file,
                          square.rank - attacker.rank};
            offset.norm();

            for (const auto &newSq : board->getPath(square, offset, true))
            {
                auto idx = board->squareToIdx(newSq);
                attackedBy[idx][i] = 1;
                m_attackedFrom[i][idx] = 1;
            }
        }
    }

    std::shared_ptr<Board> Attacks::getBoard() const
    {
        std::shared_ptr<Board> sp_pos = m_board_ptr.lock();
        if (!sp_pos)
            throw std::runtime_error("AttackingPieces could not access Postion");
        return sp_pos;
    }
} // namespace ChessGame