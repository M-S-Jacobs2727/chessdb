#include "internal/game/state.h"

#include <algorithm>
#include <sstream>
#include <stdexcept>
#include <vector>

namespace ChessGame
{
    State::State(std::string_view fenString)
    {
        std::istringstream iss{fenString.data()};
        std::string str;

        iss >> str;
        position = Position(str);

        iss >> str;
        if (str == "w")
            turn = Color::White;
        else if (str == "b")
            turn = Color::Black;
        else
            throw std::runtime_error("Invalid FEN string");

        iss >> str;
        castleRights.reset(false);
        for (const auto &c : str)
        {
            switch (c)
            {
            case 'K':
                castleRights.set(Color::White, CastleRights::Side::KING);
                break;
            case 'Q':
                castleRights.set(Color::White, CastleRights::Side::QUEEN);
                break;
            case 'k':
                castleRights.set(Color::Black, CastleRights::Side::KING);
                break;
            case 'q':
                castleRights.set(Color::Black, CastleRights::Side::QUEEN);
                break;

            default:
                throw std::runtime_error("Invalid FEN string");
            }
        }

        iss >> str;
        if (str != "-")
            enPassant = Square(str);

        iss >> str;
        halfTurnCounter = std::stoul(str);

        iss >> str;
        fullTurnCounter = std::stoul(str);
    }

    inline uint8_t State::homeRank(Color color)
    {
        return (color == Color::White) ? 0u : 7u;
    }

    /// @brief Applies a move to the board state, possibly adding information to the move
    ///     (e.g., from square, captured piece)
    /// @param move
    void State::applyMove(ChessGame::Move &move)
    {
        move.capturedPiece = position.put(move.to, move.piece);
        if (move.piece.type == PieceType::Pawn &&
            move.from.file != move.to.file &&
            move.capturedPiece.type == PieceType::None)
            move.capturedPiece = Piece{oppositeColor(move.piece.color), PieceType::Pawn};

        determineFromSquare(move);

        auto tmp = enPassant;
        determineExtra(move);
        if (tmp && enPassant && tmp.value() == enPassant.value())
            enPassant = std::nullopt;

        halfTurnCounter++;
        if (turn == Color::Black)
            fullTurnCounter++;
        turn = oppositeColor(turn);
    }

    // DANGER: REQUIRES MOVE LOGIC, NOT IMPLEMENTED CORRECTLY
    void State::determineFromSquare(Move &move)
    {
        std::vector<Square> candiateSquares;

        for (uint8_t rank = 0; rank < 8; rank++)
            for (uint8_t file = 0; file < 8; file++)
            {
                Square square{file, rank};
                if (position.get(square) == move.piece)
                    candiateSquares.push_back(square);
            }

        if (candiateSquares.size() == 0)
            throw std::runtime_error("No piece found");
        else if (candiateSquares.size() == 1)
            move.from = candiateSquares[0];
        else if (move.partialFrom.size() <= 0 || move.partialFrom.size() >= 3)
            throw std::runtime_error("No piece found");
        else if (move.partialFrom.size() == 2)
            move.from = Square(move.partialFrom);
        else
        {
            auto c = move.partialFrom[0];

            bool useFile = false;
            if ('a' <= c && c <= 'h')
                useFile = true;
            else if ('1' <= c && c <= '8')
                useFile = false;
            else
                throw std::runtime_error("No piece found");

            auto matchFunc = [c, useFile](const Square &s)
            {
                return useFile ? s.file == c - 'a' : s.rank == c - '1';
            };

            auto it = std::find_if(candiateSquares.begin(), candiateSquares.end(), matchFunc);
            if (it == candiateSquares.end())
                throw std::runtime_error("No piece found");

            auto it2 = std::find_if(it + 1, candiateSquares.end(), matchFunc);
            if (it2 != candiateSquares.end())
                throw std::runtime_error("No piece found");

            move.from = *it;
        }

        position.put(move.from, Piece{Color::White, PieceType::None});
    }

    void State::determineExtra(const Move &move)
    {
        if (move.piece.type == PieceType::Pawn)
        {
            halfTurnCounter = 0;

            if (abs(move.to.rank - move.from.rank) == 2)
                enPassant = Square{move.to.file, static_cast<uint8_t>((move.to.rank + move.from.rank) / 2)};
            else if (move.to.file != move.from.file && move.to == enPassant)
                position.put(Square{move.to.file, move.from.rank}, Piece{Color::White, PieceType::None});
            else if (move.promotedPiece.type != PieceType::None)
                position.put(move.to, move.promotedPiece);
        }
        else if (move.piece.type == PieceType::King)
        {
            if (abs(move.to.file - move.from.file) == 2)
            {
                if (move.to.file != 2u && move.to.file != 6u)
                    throw std::runtime_error("Invalid castle move");
                Square middleSquare{static_cast<uint8_t>((move.to.file + move.from.file) / 2), move.to.rank};
                Square rookSquare{static_cast<uint8_t>((move.to.file == 2u) ? 0u : 7u), move.to.rank};
                position.put(middleSquare, position.put(rookSquare, Piece{Color::White, PieceType::None}));
            }
            castleRights.remove(turn, CastleRights::Side::KING);
            castleRights.remove(turn, CastleRights::Side::QUEEN);
        }
        else if (move.piece.type == PieceType::Rook)
        {
            if (move.from.rank == homeRank(turn))
            {
                if (move.from.file == 0u)
                    castleRights.remove(turn, CastleRights::Side::QUEEN);
                else if (move.from.file == 7u)
                    castleRights.remove(turn, CastleRights::Side::KING);
            }
        }

        if (move.capturedPiece.type != PieceType::None)
        {
            halfTurnCounter = 0;
            auto opp = oppositeColor(turn);
            if (move.capturedPiece.type == PieceType::Rook && move.to.rank == homeRank(opp))
            {
                if (move.to.file == 0u)
                    castleRights.remove(opp, CastleRights::Side::QUEEN);
                else if (move.to.file == 7u)
                    castleRights.remove(opp, CastleRights::Side::KING);
            }
        }
    }
}