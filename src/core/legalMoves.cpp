#include "core/legalMoves.h"

#include <algorithm>
#include <ranges>
#include <utility>

#include "core/attacks.h"

namespace ChessGame
{
    bool squareOnPath(Square test, Square start, Square end)
    {
        Offset dir1 = end - start;
        if (!dir1.isDiagonal() && !dir1.isLateral())
            return false;

        Offset dir2 = test - start;
        if (!dir2.isDiagonal() && !dir2.isLateral())
            return false;

        // return true iff (start < test < end) or (start > test > end) or (start == test == end)
        // for both rank and file, and both end-start and test-start are diagonal or lateral paths
        return ((start.file <=> test.file) == (test.file <=> end.file)) &&
               ((start.rank <=> test.rank) == (test.rank <=> end.rank));
    }

    bool castleIsLegal(const State &state, Castling::Side side)
    {
        Square midSquare = Board::rookToSquare(state.turn, side);

        if (state.attacks.numAttackers(midSquare, oppositeColor(state.turn)) > 0)
            return false;

        // check that each square between king and rook is empty
        int file1 = (side == Castling::Side::QUEEN) ? 1 : 5,
            file2 = (side == Castling::Side::QUEEN) ? 3 : 6;
        for (auto file = file1; file <= file2; ++file)
            if (state.board.get(Square{file, midSquare.rank}))
                return false;

        return true;
    }

    bool epCatureResultsInCheck(Square from, const State &state, Square kingSq)
    {
        if (from.rank != kingSq.rank)
            return false;

        Square epPawn{state.enPassant.value().file, from.rank};
        int dir = (kingSq.file < epPawn.file) ? 1 : -1;

        auto file = kingSq.file;
        file += dir;
        while (file != epPawn.file && file != from.file)
        {
            if (state.board.get(Square{file, from.rank}))
                return false;
            file += dir;
        }

        file += dir;

        Occupant occ;
        while (0 < file && file < 7)
        {
            file += dir;
            occ = state.board.get(Square{file, from.rank});
            if (occ)
                break;
        }
        if (!occ)
            return false;
        Piece piece = occ.value();
        return (piece.color != state.turn &&
                (piece.type == PieceType::Queen ||
                 piece.type == PieceType::Rook));
    }

    std::optional<Square> getHardPin(const State &state, Square testSquare, Square kingSq)
    {
        auto pinDirection = kingSq - testSquare;
        if (!pinDirection.norm())
            return {};

        auto pathToPin = state.board.getPath(testSquare, -pinDirection, true);
        if (pathToPin.empty())
            return {};

        auto pinSquare = pathToPin.back();

        auto occupant = state.board.get(pinSquare);
        if (!occupant || occupant.value().color == state.turn)
            return {};

        Piece piece = occupant.value();

        if (piece.type == PieceType::Queen ||
            (piece.type == PieceType::Bishop && pinDirection.isDiagonal()) ||
            (piece.type == PieceType::Rook && pinDirection.isLateral()))
            return pinSquare;

        return {};
    }

    /* This is a complex function, but I'm hoping to do branching at compile time as much as possible.
     * In general, a pawn can move one square forward if the space is empty, or capture one square to
     * either foward diagonal. If it is on its starting rank, it can additionally move two squares forward
     * if both are empty. If it is able to promote, each promotion piece is a separate move, and it can
     * capture to promote as well. If it is able to capture en passant, that square must be capturable despite
     * being empty.
     */
    template <Color color>
    std::vector<Move> candidatePawnMoves(const State &state, Square from, Square kingSq)
    {
        std::vector<Move> moves{};
        constexpr Color oppColor = oppositeColor(color);
        constexpr Piece pawn{color, PieceType::Pawn};
        constexpr int pawnStartRank = static_cast<int>(color) * 5 + 1;
        constexpr int pawnToPromoteRank = static_cast<int>(color) * -5 + 6;
        constexpr int pawnToEPRank = static_cast<int>(color) * -1 + 4;

        Square oneForward = from + Offsets::forward(color);
        Square attackLeft = oneForward + Offset{-1, 0};
        Square attackRight = oneForward + Offset{1, 0};

        bool canCaptureEnPassant = (state.enPassant) &&
                                   (from.rank == pawnToEPRank) &&
                                   (abs(from.file - state.enPassant.value().file) == 1);

        if (from.rank == pawnToPromoteRank)
        {
            moves.reserve(12);

            constexpr Piece knight{color, PieceType::Knight};
            constexpr Piece bishop{color, PieceType::Bishop};
            constexpr Piece rook{color, PieceType::Rook};
            constexpr Piece queen{color, PieceType::Queen};

            if (!state.board.get(oneForward))
            {
                moves.push_back({.piece = pawn, .from = from, .to = oneForward, .promotion = knight});
                moves.push_back({.piece = pawn, .from = from, .to = oneForward, .promotion = bishop});
                moves.push_back({.piece = pawn, .from = from, .to = oneForward, .promotion = rook});
                moves.push_back({.piece = pawn, .from = from, .to = oneForward, .promotion = queen});
            }
            if (from.file != 0 && state.board.pawnCanCapture(color, attackLeft))
            {
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .promotion = knight,
                    .capture = state.board.get(attackLeft),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .promotion = bishop,
                    .capture = state.board.get(attackLeft),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .promotion = rook,
                    .capture = state.board.get(attackLeft),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .promotion = queen,
                    .capture = state.board.get(attackLeft),
                });
            }
            if (from.file != 7 && state.board.pawnCanCapture(color, attackRight))
            {
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .promotion = knight,
                    .capture = state.board.get(attackRight),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .promotion = bishop,
                    .capture = state.board.get(attackRight),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .promotion = rook,
                    .capture = state.board.get(attackRight),
                });
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .promotion = queen,
                    .capture = state.board.get(attackRight),
                });
            }
        }
        else if (from.rank == pawnStartRank)
        {
            moves.reserve(4);
            Square twoForward = oneForward + Offsets::forward(color);

            if (!state.board.get(oneForward))
            {
                moves.emplace_back(pawn, from, oneForward);
                if (!state.board.get(twoForward))
                    moves.emplace_back(pawn, from, twoForward);
            }
            if (from.file != 0 && state.board.pawnCanCapture(color, attackLeft))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .capture = state.board.get(attackLeft),
                });
            if (from.file != 7 && state.board.pawnCanCapture(color, attackRight))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .capture = state.board.get(attackRight),
                });
        }
        else if (canCaptureEnPassant)
        {
            moves.reserve(3);
            Square epAttack = state.enPassant.value();
            Square otherAttack = Square{2 * from.file - epAttack.file, epAttack.rank};

            if (!state.board.get(oneForward))
                moves.emplace_back(pawn, from, oneForward);
            if (!epCatureResultsInCheck(from, state, kingSq))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = epAttack,
                    .enPassant = true,
                    .capture = Piece{oppColor, PieceType::Pawn},
                });
            if (0 <= otherAttack.file && otherAttack.file <= 7 && state.board.pawnCanCapture(color, otherAttack))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = otherAttack,
                    .capture = state.board.get(otherAttack),
                });
        }
        else
        {
            moves.reserve(3);

            if (!state.board.get(oneForward))
                moves.emplace_back(pawn, from, oneForward);
            if (from.file != 0 && state.board.pawnCanCapture(color, attackLeft))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackLeft,
                    .capture = state.board.get(attackLeft),
                });
            if (from.file != 7 && state.board.pawnCanCapture(color, attackRight))
                moves.push_back({
                    .piece = pawn,
                    .from = from,
                    .to = attackRight,
                    .capture = state.board.get(attackRight),
                });
        }

        return moves;
    }

    template <>
    std::vector<Move> candidatePawnMoves<Color::White>(const State &, Square, Square);
    template <>
    std::vector<Move> candidatePawnMoves<Color::Black>(const State &, Square, Square);

    std::vector<Move> legalMoves(const State &state)
    {
        std::vector<Move> moves{};
        const auto &board = state.board;
        Color turnColor = state.turn;
        Color oppColor = oppositeColor(turnColor);

        Piece king{turnColor, PieceType::King};
        Square kingSq = board.kingSquare(turnColor);

        auto numCheckers = state.attacks.numAttackers(kingSq, oppColor);
        bool inCheck = numCheckers >= 1;

        std::vector<Square> toSquares = state.attacks.attacks(kingSq);
        for (const auto &to : toSquares)
            if (state.attacks.numAttackers(to, oppColor) == 0 &&
                board.canMoveTo(turnColor, to))
                moves.push_back({.piece = king, .from = kingSq, .to = to, .capture = board.get(to)});

        if (!inCheck) // castling moves
        {
            if (state.castleRights.get(turnColor, Castling::Side::QUEEN) &&
                castleIsLegal(state, Castling::Side::QUEEN))
                moves.push_back({.piece = king,
                                 .from = kingSq,
                                 .to = board.kingToSquare(turnColor, Castling::Side::QUEEN),
                                 .castle = Castling::Side::QUEEN});
            if (state.castleRights.get(turnColor, Castling::Side::KING) &&
                castleIsLegal(state, Castling::Side::KING))
                moves.push_back({.piece = king,
                                 .from = kingSq,
                                 .to = board.kingToSquare(turnColor, Castling::Side::KING),
                                 .castle = Castling::Side::KING});
        }

        if (numCheckers > 1)
            return moves;

        std::optional<Square> checker{
            (numCheckers == 0) ? std::nullopt
                               : std::make_optional(state.attacks.attackers(kingSq, oppColor)[0])};

        for (const auto &[square, occupant] : std::views::zip(board.eachSquare(), board.eachOccupant()))
        {
            if (!occupant || occupant.value().color != turnColor || occupant.value().type == PieceType::King)
                continue;

            Piece piece = occupant.value();

            auto hardPin = getHardPin(state, square, kingSq);

            std::vector<Move> candidateMoves;
            if (piece.type == PieceType::Pawn)
            {
                if (turnColor == Color::White)
                    candidatePawnMoves<Color::White>(state, square, kingSq).swap(candidateMoves);
                else
                    candidatePawnMoves<Color::Black>(state, square, kingSq).swap(candidateMoves);
            }
            else
            {
                toSquares = state.attacks.attacks(square);
                candidateMoves.reserve(toSquares.size());
                for (const auto &to : toSquares)
                    if (board.canMoveTo(turnColor, to))
                        candidateMoves.push_back({.piece = piece, .from = square, .to = to, .capture = board.get(to)});
            }

            for (const auto &move : candidateMoves)
                if ((!checker || squareOnPath(move.to, checker.value(), kingSq)) &&
                    (!hardPin || squareOnPath(move.to, hardPin.value(), kingSq)))
                    moves.push_back(move);
        }

        return moves;
    }
} // namespace ChessGame
