#include "internal/logic/legalMoves.h"
#include "internal/logic/attacks.h"
#include "internal/logic/move.h"
#include "internal/logic/offset.h"
#include "internal/logic/square.h"

#include <algorithm>
#include <array>
#include <ranges>
#include <utility>
#include <vector>

#define MAX(x, y) (x < y) ? y : x

namespace ChessGame
{
    bool castleIsLegal(const Move &move, const State &state)
    {
        auto side = (move.to.file == 2u) ? Castling::Side::QUEEN : Castling::Side::KING;
        Square midSquare{(move.from.file + move.to.file) / 2, move.from.rank};
        if (!state.castleRights.get(state.turn, side) ||
            state.attacks.numAttackers(midSquare, oppositeColor(state.turn)) > 0)
            return false;

        if (side == Castling::Side::QUEEN)
        {
            for (auto file = 1; file <= 3; ++file)
                if (state.board.get(Square{file, move.from.rank}))
                    return false;
        }
        else
        {
            for (auto file = 5; file <= 6; ++file)
                if (state.board.get(Square{file, move.from.rank}))
                    return false;
        }

        return true;
    }

    bool moveDisallowedByCheck(const Move &move,
                               const Board &board,
                               Square kingSq,
                               Square checkingSq)
    {
        if (move.to == checkingSq)
            return true;

        Piece checkingPiece = board.get(checkingSq).piece();

        // Cannot block these pieces without capturing
        if (checkingPiece.type == PieceType::Pawn ||
            checkingPiece.type == PieceType::Knight ||
            checkingPiece.type == PieceType::King)
            return false;

        Offset dir{kingSq.file - checkingSq.file,
                   kingSq.rank - checkingSq.rank};
        dir /= MAX(abs(dir.file), abs(dir.rank));

        auto path = board.getPath(checkingSq, dir, false);
        return (std::find(path.begin(), path.end(), move.to) == path.end());
    }

    bool moveDisallowedByHardPin(const Move &move, const std::pair<Square, Offset> &hardPin, Square kingSq)
    {
        for (Square tmpSq = hardPin.first; tmpSq != kingSq; tmpSq = hardPin.second(tmpSq).value_or(kingSq))
            if (tmpSq == move.to)
                return false;
        return true;
    }

    bool epCatureResultsInCheck(const Move &move, const State &state, Square kingSq)
    {
        auto rank = move.from.rank;
        Square epPawn = state.enPassant.value() + Offsets::backward(state.turn);
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
            if (state.board.get(Square{file, rank}))
                return false;
            file += dir;
        }

        file += dir * 2;

        while (0 <= file && file <= 7)
        {
            if (state.board.get(Square{file, rank}))
            {
                Piece piece = state.board.get(Square{file, rank}).piece();
                if (piece.color != state.turn &&
                    (piece.type == PieceType::Queen ||
                     piece.type == PieceType::Rook))
                    return true;
                return false;
            }
        }
        return false;
    }

    std::unordered_set<Move> candidatePawnMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(4);
        Piece pawn{color, PieceType::Pawn};
        uint8_t pawnStartRank = static_cast<uint8_t>(static_cast<int>(color) * -5 + 6);

        Offset oneForward = Offsets::forward(color);
        Offset twoForward = oneForward * 2;
        Offset attackLeft = oneForward + Offset{-1, 0};
        Offset attackRight = oneForward + Offset{1, 0};

        auto to = from + oneForward;
        if (board.valid(to) && !board.get(to))
            moves.emplace(pawn, from, to);

        to = from + twoForward;
        if (board.valid(to) && from.rank == pawnStartRank && !board.get(to))
            moves.emplace(pawn, from, to);

        to = from + attackLeft;
        if (board.valid(to) && (!board.get(to) || board.get(to).piece().color != color))
            moves.emplace(pawn, from, to);

        to = from + attackRight;
        if (board.valid(to) && (!board.get(to) || board.get(to).piece().color != color))
            moves.emplace(pawn, from, to);

        return moves;
    }

    std::unordered_set<Move> candidateKnightMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(8);

        for (const auto &offset : Offsets::knight)
        {
            auto to = from + offset;
            if (board.valid(to))
            {
                auto occupant = board.get(to);
                if (!occupant || occupant.piece().color != color)
                    moves.emplace(Piece{color, PieceType::Knight}, from, to);
            }
        }
        return moves;
    }

    std::unordered_set<Move> candidateBishopMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : Offsets::bishop)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Bishop}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace(Piece{color, PieceType::Bishop}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateRookMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : Offsets::rook)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Rook}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace(Piece{color, PieceType::Rook}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateQueenMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(32);

        for (const auto &offset : Offsets::queenKing)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace(Piece{color, PieceType::Queen}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace(Piece{color, PieceType::Queen}, from, last);
        }
        return moves;
    }

    std::unordered_set<Move> candidateKingMoves(const Board &board, Square from, Color color)
    {
        std::unordered_set<Move> moves{};
        moves.reserve(8);
        Piece king{color, PieceType::King};
        auto hr = homeRank(color);

        for (const auto &offset : Offsets::queenKing)
        {
            auto to = from + offset;
            if (board.valid(to))
            {
                auto occupant = board.get(to);
                if (!occupant || occupant.piece().color != color)
                    moves.emplace(king, from, to);
            }
        }
        if (from == Square{4, hr})
        {
            Square to{2, hr};
            if (!board.get(to))
                moves.emplace(king, from, to);
            to = Square{6, hr};
            if (!board.get(to))
                moves.emplace(king, from, to);
        }
        return moves;
    }

    std::unordered_set<Move> candidateMoves(const Board &board, Square square)
    {
        if (!board.get(square))
            return {};

        std::unordered_set<Move> moves{};
        Piece piece = board.get(square).piece();

        switch (piece.type)
        {
        case PieceType::Pawn:
            candidatePawnMoves(board, square, piece.color).swap(moves);
            break;

        case PieceType::Knight:
            candidateKnightMoves(board, square, piece.color).swap(moves);
            break;

        case PieceType::Bishop:
            candidateBishopMoves(board, square, piece.color).swap(moves);
            break;

        case PieceType::Rook:
            candidateRookMoves(board, square, piece.color).swap(moves);
            break;

        case PieceType::Queen:
            candidateQueenMoves(board, square, piece.color).swap(moves);
            break;

        case PieceType::King:
            candidateKingMoves(board, square, piece.color).swap(moves);
            break;

        default:
            std::unreachable();
            break;
        }

        return moves;
    }

    std::optional<std::pair<Square, Offset>> getHardPin(const Board &board, Square square, Color color)
    {
        auto kingSq = board.kingSquare(color);
        auto offset = square - kingSq;
        if (!offset.norm())
            return {};

        Color oppColor = oppositeColor(color);

        auto pathToPin = board.getPath(square, -offset, true);
        if (pathToPin.empty())
            return {};

        auto pinSquare = pathToPin.back();
        std::pair<Square, Offset> hardPin{pinSquare, offset};

        auto occupant = board.get(pinSquare);
        if (!occupant || occupant.piece().color == color)
            return {};

        Piece piece = occupant.piece();

        if (piece.type == PieceType::Queen ||
            piece.type == PieceType::Bishop && offset.isDiagonal() ||
            piece.type == PieceType::Rook && offset.isLateral())
            return hardPin;

        return {};
    }

    std::unordered_set<Move> legalMoves(const State &state)
    {
        std::unordered_set<Move> moves{};
        const auto &board = state.board;
        Color color = state.turn;
        Color oppColor = oppositeColor(color);

        Square kingSq = board.kingSquare(color);

        auto numCheckers = state.attacks.numAttackers(kingSq, oppColor);
        bool inCheck = numCheckers >= 1;

        std::unordered_set<Move> kingMoves = candidateKingMoves(board, kingSq, color);
        for (const auto &move : kingMoves)
        {
            if (state.attacks.numAttackers(move.to, oppColor) > 0)
                continue;

            if (move.castle && (inCheck || !castleIsLegal(move, state)))
                continue;

            moves.insert(move);
        }

        if (numCheckers > 1)
            return moves;

        std::optional<Square> checker{
            (numCheckers == 0) ? std::nullopt
                               : std::make_optional(state.attacks.attackers(kingSq, oppColor)[0])};

        for (const auto &[sq, p] : std::views::zip(board.eachSquare(), board.eachOccupant()))
        {
            if (!p || p.piece().color != color || p.piece().type == PieceType::King)
                continue;

            Piece piece = p.piece();

            // use concepts(?) to affirm that board.get(hardPin.value()).has_value() if hardPin.has_value()
            auto hardPin = getHardPin(board, sq, color);
            bool canCaptureEnPassant = (state.enPassant) &&
                                       (piece.type == PieceType::Pawn) &&
                                       (sq.rank == ((color == Color::White) ? 5u : 4u)) &&
                                       (abs(sq.file - state.enPassant.value().file) == 1);

            for (const auto &move : candidateMoves(board, sq))
            {
                if (checker && moveDisallowedByCheck(move, board, kingSq, checker.value()))
                    continue;

                if (hardPin && moveDisallowedByHardPin(move, hardPin.value(), kingSq))
                    continue;

                if (canCaptureEnPassant && epCatureResultsInCheck(move, state, kingSq))
                    continue;

                moves.insert(move);
            }
        }

        return moves;
    }
} // namespace ChessGame
