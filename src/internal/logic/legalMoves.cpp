#include "internal/logic/legalMoves.h"
#include "internal/logic/offset.h"

#include "legalMoves.h"
#include <algorithm>
#include <array>

#define MAX(x, y) (x < y) ? y : x

namespace ChessGame
{
    bool castleIsLegal(const Move &move, const State &state, const AttackedSquares &attacked)
    {
        auto side = (move.to.file == 2u) ? CastleSide::QUEEN : CastleSide::KING;
        Square midSquare{(move.from.file + move.to.file) / 2, move.from.rank};
        if (!state.castleRights.get(state.turn, side) ||
            attacked.attackers(midSquare).size() > 0)
            return false;

        if (side == CastleSide::QUEEN)
        {
            for (auto file = 1; file <= 3; ++file)
                if (state.position.get(Square{file, move.from.rank}))
                    return false;
        }
        else
        {
            for (auto file = 5; file <= 6; ++file)
                if (state.position.get(Square{file, move.from.rank}))
                    return false;
        }

        return true;
    }

    bool moveDisallowedByCheck(const Move &move,
                               const Position &pos,
                               Square kingSq,
                               Square checkingSq)
    {
        if (move.to == checkingSq)
            return true;

        Piece checkingPiece = pos.get(checkingSq).value();

        // Cannot block these pieces without capturing
        if (checkingPiece.type == PieceType::Pawn ||
            checkingPiece.type == PieceType::Knight ||
            checkingPiece.type == PieceType::King)
            return false;

        Offset dir{kingSq.file - checkingSq.file,
                   kingSq.rank - checkingSq.rank};
        dir /= MAX(abs(dir.file), abs(dir.rank));

        auto path = pos.getPath(checkingSq, dir, false);
        return (std::find(path.begin(), path.end(), move.to) == path.end());
    }

    bool moveDisallowedByHardPin(const Move &move, Square pinningSquare, Offset pinDir, Square kingSq)
    {
        for (Square tmpSq = pinningSquare; tmpSq != kingSq; tmpSq = pinDir(tmpSq).value_or(kingSq))
            if (tmpSq == move.to)
                return false;
        return true;
    }

    bool epCatureResultsInCheck(const Move &move, const State &state, Square kingSq)
    {
        auto rank = move.from.rank;
        Square epPawn = backward(state.turn)(state.enPassant.value()).value();
        Square myPawn = move.from;
        int dir = (kingSq.file < epPawn.file) ? 1 : -1;
        Offset pawnToKingDir{kingSq.file - epPawn.file,
                             kingSq.rank - epPawn.rank};

        if (epPawn.rank != kingSq.rank)
            return false;

        bool secondHalf = false;
        auto file = kingSq.file;
        file += dir;
        while (file != epPawn.file && file != myPawn.file)
        {
            if (state.position.get(Square{file, rank}))
                return false;
            file += dir;
        }

        file += dir * 2;

        while (0 <= file && file <= 7)
        {
            if (state.position.get(Square{file, rank}))
            {
                Piece piece = state.position.get(Square{file, rank}).value();
                if (piece.color != state.turn &&
                    (piece.type == PieceType::Queen ||
                     piece.type == PieceType::Rook))
                    return true;
                return false;
            }
        }
        return false;
    }

    std::unordered_set<Move> legalMoves(const State &state, const AttackedSquares &attacked)
    {
        std::unordered_set<Move> moves{};
        const auto &pos = state.position;
        Color color = state.turn;

        Square kingSq = pos.kingSquare(color);

        auto attackers = attacked.attackers(kingSq);
        bool inCheck = attackers.size() >= 1;

        std::unordered_set<Move> kingMoves = candidateKingMoves(pos, kingSq, color);
        for (const auto &move : kingMoves)
        {
            if (attacked.attackers(move.to).size() > 0)
                continue;

            if (move.castle && (inCheck || !castleIsLegal(move, state, attacked)))
                continue;

            moves.insert(move);
        }

        if (attackers.size() > 1)
            return moves;

        std::optional<Square> checker{
            attackers.empty() ? std::nullopt
                              : std::make_optional(*(attackers.begin()))};

        for (const auto &[sq, p] : pos.eachSquare())
        {
            if (!p || p.value().color != color || p.value().type == PieceType::King)
                continue;

            Piece piece = p.value();

            // use concepts(?) to affirm that pos.get(hardPin.value()).has_value() if hardPin.has_value()
            auto hardPin = getHardPin(pos, sq, color);
            bool canCaptureEnPassant = (state.enPassant) &&
                                       (piece.type == PieceType::Pawn) &&
                                       (sq.rank == ((color == Color::White) ? 5u : 4u)) &&
                                       (abs(sq.file - state.enPassant.value().file) == 1);

            for (const auto &move : candidateMoves(pos, sq))
            {
                if (checker && moveDisallowedByCheck(move, pos, kingSq, checker.value()))
                    continue;

                if (hardPin && moveDisallowedByHardPin(move, hardPin.value().first, hardPin.value().second, kingSq))
                    continue;

                if (canCaptureEnPassant && epCatureResultsInCheck(move, state, kingSq))
                    continue;

                moves.insert(move);
            }
        }

        return moves;
    }

    std::unordered_set<Move> candidateMoves(const Position &pos, Square square)
    {
        if (!pos.get(square))
            return {};

        std::unordered_set<Move> moves{};
        Piece piece = pos.get(square).value();

        switch (piece.type)
        {
        case PieceType::Pawn:
            candidatePawnMoves(pos, square, piece.color).swap(moves);
            break;

        case PieceType::Knight:
            candidateKnightMoves(pos, square, piece.color).swap(moves);
            break;

        case PieceType::Bishop:
            candidateBishopMoves(pos, square, piece.color).swap(moves);
            break;

        case PieceType::Rook:
            candidateRookMoves(pos, square, piece.color).swap(moves);
            break;

        case PieceType::Queen:
            candidateQueenMoves(pos, square, piece.color).swap(moves);
            break;

        case PieceType::King:
            candidateKingMoves(pos, square, piece.color).swap(moves);
            break;

        default:
            std::unreachable();
            break;
        }

        return moves;
    }

    std::unordered_set<Move> candidatePawnMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(4);
        Piece pawn{color, PieceType::Pawn};
        uint8_t pawnStartRank = static_cast<uint8_t>(static_cast<int>(color) * -5 + 6);

        Offset oneForward = forward(color);
        Offset twoForward = oneForward * 2;
        Offset attackLeft = oneForward + Offset{-1, 0};
        Offset attackRight = oneForward + Offset{1, 0};

        auto to = oneForward(from);
        if (to && !pos.get(to.value()))
            moves.emplace(pawn, from, to.value());

        to = twoForward(from);
        if (to && from.rank == pawnStartRank && !pos.get(to.value()))
            moves.emplace(pawn, from, to.value());

        to = attackLeft(from);
        if (to && (!pos.get(to.value()) || pos.get(to.value()).value().color != color))
            moves.emplace(pawn, from, to.value());

        to = attackRight(from);
        if (to && (!pos.get(to.value()) || pos.get(to.value()).value().color != color))
            moves.emplace(pawn, from, to.value());

        return moves;
    }

    std::unordered_set<Move> candidateKnightMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(8);

        for (const auto &offset : knightOffsets)
        {
            auto to = offset(from);
            if (to)
            {
                auto piece = pos.get(to.value());
                if (!piece || piece.value().color != color)
                    moves.emplace(Piece{color, PieceType::Knight}, from, to.value());
            }
        }
        return moves;
    }

    std::unordered_set<Move> candidateBishopMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : bishopOffsets)
        {
            auto path = pos.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Bishop}, from, to);
            auto piece = pos.get(last);
            if (!piece || piece.value().color != color)
                moves.emplace(Piece{color, PieceType::Bishop}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateRookMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : rookOffsets)
        {
            auto path = pos.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Rook}, from, to);
            auto piece = pos.get(last);
            if (!piece || piece.value().color != color)
                moves.emplace(Piece{color, PieceType::Rook}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateQueenMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(32);

        for (const auto &offset : queenKingOffsets)
        {
            auto path = pos.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Queen}, from, to);
            auto piece = pos.get(last);
            if (!piece || piece.value().color != color)
                moves.emplace(Piece{color, PieceType::Queen}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateKingMoves(const Position &pos, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(8);
        Piece king{color, PieceType::King};
        auto hr = homeRank(color);

        for (const auto &offset : queenKingOffsets)
        {
            auto to = offset(from);
            if (to)
            {
                auto piece = pos.get(to.value());
                if (!piece || piece.value().color != color)
                    moves.emplace(king, from, to.value());
            }
        }
        if (from == Square{4, hr})
        {
            Square to{2, hr};
            if (!pos.get(to))
                moves.emplace(king, from, to);
            to = Square{6, hr};
            if (!pos.get(to))
                moves.emplace(king, from, to);
        }
        return moves;
    }
} // namespace ChessGame
