#include "internal/logic/state.h"

#include <algorithm>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <vector>

#include "internal/logic/offset.h"

namespace ChessGame
{
    const std::unordered_map<char, PieceType> PGNPieceType{
        {'K', PieceType::King},
        {'O', PieceType::King},
        {'Q', PieceType::Queen},
        {'R', PieceType::Rook},
        {'B', PieceType::Bishop},
        {'N', PieceType::Knight},
        {'a', PieceType::Pawn},
        {'b', PieceType::Pawn},
        {'c', PieceType::Pawn},
        {'d', PieceType::Pawn},
        {'e', PieceType::Pawn},
        {'f', PieceType::Pawn},
        {'g', PieceType::Pawn},
        {'h', PieceType::Pawn}};

    State::State() : board(initFEN), attacks(std::make_shared<Board>(board))
    {
    }

    void State::applyMove(const ChessGame::Move &move)
    {
        board.remove(move.from);
        board.put(move.to,
                  move.promotion ? Piece{turn, move.promotion.value()}
                                 : move.piece);

        if (move.piece.type == PieceType::Pawn || move.capture)
            halfTurnCounter = 0;
        else
            ++halfTurnCounter;

        if (move.castle)
        {
            bool qs = move.castle.value() == Castling::Side::QUEEN;
            Square rookFrom{qs ? 0 : 7, move.from.rank},
                rookTo{qs ? 3 : 5, move.from.rank};
            auto rook = board.remove(rookFrom);
            board.put(rookTo, rook.piece());

            castleRights.remove(turn);
        }
        else if (move.enPassant)
        {
            Square capturedPawnSq = move.to + Offsets::backward(turn);
            board.remove(capturedPawnSq);
            enPassant = std::nullopt;
        }

        if (move.piece.type == PieceType::Pawn &&
            2 == abs(move.to.rank - move.from.rank) &&
            move.to.file == move.from.file)
            enPassant = Square{move.from.file, (move.from.rank + move.to.rank) / 2};
        else
            enPassant = std::nullopt;

        auto r = homeRank(turn);
        if (move.piece.type == PieceType::Rook && move.from.rank == r)
        {
            if (move.from.file == 0)
                castleRights.remove(turn, Castling::Side::QUEEN);
            if (move.from.file == 7)
                castleRights.remove(turn, Castling::Side::KING);
        }
        if (turn == Color::Black)
            fullTurnCounter++;
        turn = oppositeColor(turn);

        attacks.applyMove(move);
    }
}