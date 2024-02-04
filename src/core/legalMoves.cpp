#include "core/legalMoves.h"

#include <algorithm>
#include <ranges>
#include <utility>

#include "core/attacks.h"

namespace ChessGame
{
    bool castleIsLegal(const Move &move, const State &state)
    {
        auto side = (move.to.file == 2u) ? Castling::Side::QUEEN : Castling::Side::KING;
        Square midSquare{(move.from.file + move.to.file) / 2, move.from.rank};
        if (!state.castleRights.get(state.turn, side) ||
            state.attacks.numAttackers(midSquare, oppositeColor(state.turn)) > 0)
            return false;

        int file1 = (side == Castling::Side::QUEEN) ? 1 : 5,
            file2 = (side == Castling::Side::QUEEN) ? 3 : 6;
        for (auto file = file1; file <= file2; ++file)
            if (state.board.get(Square{file, move.from.rank}))
                return false;

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
        return std::find(path.begin(), path.end(), move.to) == path.end();
    }

    bool moveDisallowedByHardPin(const Move &move, const HardPin &hardPin, Square kingSq, const Board &board)
    {
        for (Square tmpSq = hardPin.attacker; board.valid(tmpSq) && tmpSq != kingSq; tmpSq += hardPin.direction)
            if (tmpSq == move.to)
                return false;
        return true;
    }

    bool epCatureResultsInCheck(const Move &move, const State &state, Square kingSq)
    {
        auto rank = move.from.rank;
        Square epPawn = state.enPassant.value() + Offsets::backward(state.turn);

        if (epPawn.rank != kingSq.rank)
            return false;

        Square myPawn = move.from;
        int dir = (kingSq.file < epPawn.file) ? 1 : -1;

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

    std::vector<Move> candidatePawnMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(4);
        Piece pawn{color, PieceType::Pawn};
        uint8_t pawnStartRank = static_cast<uint8_t>(static_cast<int>(color) * -5 + 6);

        Offset oneForward = Offsets::forward(color);
        Offset twoForward = oneForward * 2;
        Offset attackLeft = oneForward + Offset{-1, 0};
        Offset attackRight = oneForward + Offset{1, 0};

        auto to = from + oneForward;
        if (board.valid(to) && !board.get(to))
            moves.emplace_back(pawn, from, to);

        to = from + twoForward;
        if (board.valid(to) && from.rank == pawnStartRank && !board.get(to))
            moves.emplace_back(pawn, from, to);

        to = from + attackLeft;
        if (board.valid(to) && (!board.get(to) || board.get(to).piece().color != color))
            moves.emplace_back(pawn, from, to);

        to = from + attackRight;
        if (board.valid(to) && (!board.get(to) || board.get(to).piece().color != color))
            moves.emplace_back(pawn, from, to);

        return moves;
    }

    std::vector<Move> candidateKnightMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(8);

        for (const auto &offset : Offsets::knight)
        {
            auto to = from + offset;
            if (board.valid(to))
            {
                auto occupant = board.get(to);
                if (!occupant || occupant.piece().color != color)
                    moves.emplace_back(Piece{color, PieceType::Knight}, from, to);
            }
        }
        return moves;
    }

    std::vector<Move> candidateBishopMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : Offsets::bishop)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace_back(Piece{color, PieceType::Bishop}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace_back(Piece{color, PieceType::Bishop}, from, last);
        }
        return moves;
    }

    std::vector<Move> candidateRookMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(16);

        for (const auto &offset : Offsets::rook)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace_back(Piece{color, PieceType::Rook}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace_back(Piece{color, PieceType::Rook}, from, last);
        }
        return moves;
    }

    std::vector<Move> candidateQueenMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(32);

        for (const auto &offset : Offsets::queenKing)
        {
            auto path = board.getPath(from, offset, true);
            auto last = path.back();
            path.pop_back();
            for (const auto &to : path)
                moves.emplace_back(Piece{color, PieceType::Queen}, from, to);
            auto occupant = board.get(last);
            if (!occupant || occupant.piece().color != color)
                moves.emplace_back(Piece{color, PieceType::Queen}, from, last);
        }
        return moves;
    }

    std::vector<Move> candidateKingMoves(const Board &board, Square from, Color color)
    {
        std::vector<Move> moves{};
        moves.reserve(8);
        Piece king{color, PieceType::King};
        auto hr = board.homeRank(color);

        for (const auto &offset : Offsets::queenKing)
        {
            auto to = from + offset;
            if (board.valid(to))
            {
                auto occupant = board.get(to);
                if (!occupant || occupant.piece().color != color)
                    moves.emplace_back(king, from, to);
            }
        }
        if (from == Square{4, hr})
        {
            Square to{2, hr};
            if (!board.get(to))
                moves.emplace_back(king, from, to);
            to = Square{6, hr};
            if (!board.get(to))
                moves.emplace_back(king, from, to);
        }
        return moves;
    }

    std::vector<Move> candidateMoves(const Board &board, Square square)
    {
        if (!board.get(square))
            return {};

        Piece piece = board.get(square).piece();
        std::vector<Move> moves{};

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

    std::optional<HardPin> getHardPin(const Board &board, Square square, Color color)
    {
        auto kingSq = board.kingSquare(color);
        auto offset = square - kingSq;
        if (!offset.norm())
            return {};

        auto pathToPin = board.getPath(square, -offset, true);
        if (pathToPin.empty())
            return {};

        auto attacker = pathToPin.back();
        HardPin hardPin{attacker, offset};

        auto occupant = board.get(attacker);
        if (!occupant || occupant.piece().color == color)
            return {};

        Piece piece = occupant.piece();

        if (piece.type == PieceType::Queen ||
            (piece.type == PieceType::Bishop && offset.isDiagonal()) ||
            (piece.type == PieceType::Rook && offset.isLateral()))
            return hardPin;

        return {};
    }

    std::vector<Move> legalMoves(const State &state)
    {
        std::vector<Move> moves{};
        const auto &board = state.board;
        Color color = state.turn;
        Color oppColor = oppositeColor(color);

        Square kingSq = board.kingSquare(color);

        auto numCheckers = state.attacks.numAttackers(kingSq, oppColor);
        bool inCheck = numCheckers >= 1;

        std::vector<Move> kingMoves = candidateKingMoves(board, kingSq, color);
        for (const auto &move : kingMoves)
        {
            if (state.attacks.numAttackers(move.to, oppColor) > 0)
                continue;

            if (move.castle && (inCheck || !castleIsLegal(move, state)))
                continue;

            moves.push_back(move);
        }

        if (numCheckers > 1)
            return moves;

        std::optional<Square> checker{
            (numCheckers == 0) ? std::nullopt
                               : std::make_optional(state.attacks.attackers(kingSq, oppColor)[0])};

        for (const auto &[square, occupant] : std::views::zip(board.eachSquare(), board.eachOccupant()))
        {
            if (!occupant || occupant.piece().color != color || occupant.piece().type == PieceType::King)
                continue;

            Piece piece = occupant.piece();

            auto hardPin = getHardPin(board, square, color);
            bool canCaptureEnPassant = (state.enPassant) &&
                                       (piece.type == PieceType::Pawn) &&
                                       (square.rank == ((color == Color::White) ? 5 : 4)) &&
                                       (abs(square.file - state.enPassant.value().file) == 1);

            for (const auto &move : candidateMoves(board, square))
            {
                if (checker && moveDisallowedByCheck(move, board, kingSq, checker.value()))
                    continue;

                if (hardPin && moveDisallowedByHardPin(move, hardPin.value(), kingSq, board))
                    continue;

                if (canCaptureEnPassant && epCatureResultsInCheck(move, state, kingSq))
                    continue;

                moves.push_back(move);
            }
        }

        return moves;
    }
} // namespace ChessGame
