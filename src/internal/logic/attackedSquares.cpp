#include "attackedSquares.h"

#include <array>

#include "internal/logic/offset.h"

namespace ChessGame
{
    constexpr std::array<Offset, 2> pawnOffsets{
        Offset{-1, 1},
        Offset{1, 1}};
    constexpr std::array<Offset, 8> knightOffsets{
        Offset{-2, -1},
        Offset{-1, -2},
        Offset{1, -2},
        Offset{2, -1},
        Offset{2, 1},
        Offset{1, 2},
        Offset{-1, 2},
        Offset{-2, 1}};
    constexpr std::array<Offset, 4> bishopOffsets{
        Offset{-1, -1},
        Offset{-1, 1},
        Offset{1, -1},
        Offset{1, 1}};
    constexpr std::array<Offset, 4> rookOffsets{
        Offset{-1, 0},
        Offset{1, 0},
        Offset{0, -1},
        Offset{0, 1}};
    constexpr std::array<Offset, 8> queenOffsets{
        Offset{-1, -1},
        Offset{-1, 1},
        Offset{1, -1},
        Offset{1, 1},
        Offset{-1, 0},
        Offset{1, 0},
        Offset{0, -1},
        Offset{0, 1}};

    AttackedSquares::AttackedSquares(Color color, std::shared_ptr<Position> pos)
        : m_color(color), m_pos(pos), m_squares()
    {
        for (auto &sq : m_squares)
            sq.reserve(16);
        recompute();
    }

    void AttackedSquares::recompute()
    {
        auto pos = getPos();
        for (auto &sq : m_squares)
            sq.clear();

        for (const auto &[sq, piece] : pos->eachSquare())
            if (piece && piece.value().color == m_color)
                addAttacker(sq, piece.value().type);
    }

    const std::unordered_set<Square> &AttackedSquares::attackers(Square sq) const
    {
        return m_squares[sq.idx()];
    }

    bool AttackedSquares::attacked(Square sq) const
    {
        return !m_squares[sq.idx()].empty();
    }

    void AttackedSquares::applyMove(const Move &move)
    {
        bool attacking = move.piece.color == m_color;

        removePiece(move.from);
        if (!move.capture)
            addPiece(move.to);
        if (attacking)
        {
            removeAttacker(move.from, move.piece.type);
            addAttacker(move.to, move.promotion ? move.promotion.value() : move.piece.type);
        }

        if (move.enPassant)
        {
            Square capturedPawnSq = backward(m_color)(move.to).value();
            removePiece(capturedPawnSq);
            if (!attacking)
                removeAttacker(capturedPawnSq, PieceType::Pawn);
        }
        else if (move.castle)
        {
            bool qs = move.castle.value() == CastleSide::QUEEN;
            Square rookTo = Square{qs ? 3 : 5, move.from.rank},
                   rookFrom = Square{qs ? 0 : 7, move.from.rank};
            removePiece(rookFrom);
            addPiece(rookTo);
            if (attacking)
            {
                removeAttacker(rookFrom, PieceType::Rook);
                addAttacker(rookTo, PieceType::Rook);
            }
        }
        else if (move.capture && !attacking)
        {
            removeAttacker(move.to, move.capture.value());
        }
    }

    void AttackedSquares::addAttacker(Square sq, PieceType pt)
    {
        std::optional<Square> maybeSq = std::nullopt;
        Square newSq{};
        auto pos = getPos();
        switch (pt)
        {
        case PieceType::Pawn:
            for (auto offset : pawnOffsets)
            {
                if (m_color == Color::White)
                    offset.rank = -1;
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].insert(sq);
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : knightOffsets)
            {
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].insert(sq);
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : bishopOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].insert(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::Rook:
            for (const auto &offset : rookOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].insert(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::Queen:
            for (const auto &offset : queenOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].insert(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::King:
            for (const auto &offset : queenOffsets)
            {
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].insert(sq);
            }
            break;

        default:
            std::unreachable();
        }
    }

    void AttackedSquares::removeAttacker(Square sq, PieceType pt)
    {
        std::optional<Square> maybeSq = std::nullopt;
        Square newSq{};
        auto pos = getPos();
        switch (pt)
        {
        case PieceType::Pawn:
            for (auto offset : pawnOffsets)
            {
                if (m_color == Color::White)
                    offset.rank = -1;
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].erase(sq);
            }
            break;

        case PieceType::Knight:
            for (const auto &offset : knightOffsets)
            {
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].erase(sq);
            }
            break;

        case PieceType::Bishop:
            for (const auto &offset : bishopOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].erase(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::Rook:
            for (const auto &offset : rookOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].erase(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::Queen:
            for (const auto &offset : queenOffsets)
            {
                for (const auto &newSq : pos->getPath(sq, offset))
                {
                    m_squares[newSq.idx()].erase(sq);
                    if (pos->get(newSq))
                        break;
                }
            }
            break;

        case PieceType::King:
            for (const auto &offset : queenOffsets)
            {
                maybeSq = offset(sq);
                if (maybeSq)
                    m_squares[maybeSq.value().idx()].erase(sq);
            }
            break;

        default:
            std::unreachable();
        }
    }

    void AttackedSquares::addPiece(Square sq)
    {
        auto pos = getPos();
        auto attackers = m_squares[sq.idx()];
        Offset offset{};
        if (attackers.empty())
            return;

        for (const auto &attSq : attackers)
        {
            bool follow = false;
            auto piece = pos->get(attSq).value(); // TODO: throw with message

            switch (piece.type)
            {
            case PieceType::Bishop:
            case PieceType::Rook:
            case PieceType::Queen:
                offset = {sq.file - attSq.file,
                          sq.rank - attSq.rank};
                offset /= abs(offset.file);
                follow = true;
                break;

            default:
                break;
            }
            if (!follow)
                continue;

            for (const auto &newSq : pos->getPath(sq, offset))
                m_squares[newSq.idx()].erase(sq);
        }
    }

    void AttackedSquares::removePiece(Square sq)
    {
        auto pos = getPos();
        auto attackers = m_squares[sq.idx()];
        Offset offset{};
        if (attackers.empty())
            return;

        for (const auto &attSq : attackers)
        {
            bool follow = false;
            auto piece = pos->get(attSq).value(); // TODO: throw with message

            switch (piece.type)
            {
            case PieceType::Bishop:
            case PieceType::Rook:
            case PieceType::Queen:
                offset = {sq.file - attSq.file,
                          sq.rank - attSq.rank};
                offset /= abs(offset.file);
                follow = true;
                break;

            default:
                break;
            }
            if (!follow)
                continue;

            for (const auto &newSq : pos->getPath(sq, offset))
            {
                m_squares[newSq.idx()].insert(sq);
                if (pos->get(newSq))
                    break;
            }
        }
    }

    std::shared_ptr<Position> AttackedSquares::getPos() const
    {
        std::shared_ptr<Position> sp_pos = m_pos.lock();
        if (!sp_pos)
            throw std::runtime_error("AttackingPieces could not access Postion");
        return sp_pos;
    }
} // namespace ChessGame