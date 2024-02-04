#include "core/state.h"

#include <memory>
#include <sstream>
#include <stdexcept>

#include "core/offset.h"

namespace ChessGame
{
    State::State()
        : board(FEN::startpos), fullTurnCounter(1), halfTurnCounter(0),
          attacks(std::make_shared<Board>(board))
    {
    }

    State::State(std::string_view fenstr)
        : board(fenstr), attacks(std::make_shared<Board>(board))
    {
        std::istringstream readFEN{fenstr.data()};

        std::string word;
        readFEN >> word;

        readFEN >> word;
        if (word.size() != 1)
            throw std::runtime_error("Invalid turn string.");
        switch (word[0])
        {
        case 'w':
            turn = Color::White;
            break;
        case 'b':
            turn = Color::Black;
            break;

        default:
            throw std::runtime_error("Invalid turn string.");
            break;
        }

        readFEN >> word;
        castleRights = Castling::Rights(word);

        readFEN >> word;
        if (word != "-")
        {
            if (word.size() != 2)
                throw std::runtime_error("Invalid en passant square in FEN string.");
            enPassant = Square{word[0] - 'a', word[1] - '1'};
        }

        readFEN >> word;
        halfTurnCounter = std::stoul(word);
        readFEN >> word;
        fullTurnCounter = std::stoul(word);
    }

    void State::applyMove(const ChessGame::Move &move)
    {
        board.remove(move.from);
        board.put(move.to, move.promotion.value_or(move.piece));

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
            board.put(rookTo, rook.value());

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

        auto r = board.homeRank(turn);
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

    std::string State::toFEN() const
    {
        auto fenstr = board.toFen();
        fenstr += ' ';
        fenstr += (turn == Color::White) ? 'w' : 'b';
        fenstr += ' ';
        fenstr += castleRights.toFEN();
        fenstr += ' ';
        if (enPassant)
        {
            fenstr += static_cast<char>('a' + enPassant.value().file);
            fenstr += static_cast<char>('1' + enPassant.value().rank);
        }
        else
            fenstr += '-';
        fenstr += ' ';
        fenstr += std::to_string(halfTurnCounter);
        fenstr += ' ';
        fenstr += std::to_string(fullTurnCounter);

        return fenstr;
    }
}