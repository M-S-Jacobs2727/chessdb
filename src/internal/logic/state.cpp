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
        castleRights = CastleRights(str);

        iss >> str;
        if (str != "-")
            enPassant = Square(str);

        iss >> str;
        halfTurnCounter = std::stoul(str);

        iss >> str;
        fullTurnCounter = std::stoul(str);
    }

    /// @brief Applies a move to the board state, possibly adding information to the move
    ///     (e.g., from square, captured piece)
    /// @param move
    void State::applyMove(const ChessGame::Move &move)
    {
        position.remove(move.from);
        position.put(move.to,
                     move.promotion ? Piece{turn, move.promotion.value()}
                                    : move.piece);

        if (move.piece.type == PieceType::Pawn || move.capture)
            halfTurnCounter = 0;
        else
            ++halfTurnCounter;

        if (move.castle)
        {
            bool qs = move.castle.value() == CastleSide::QUEEN;
            Square rookFrom{qs ? 0 : 7, move.from.rank},
                rookTo{qs ? 3 : 5, move.from.rank};
            auto rook = position.remove(rookFrom);
            position.put(rookTo, rook.value());

            castleRights.remove(turn, CastleSide::QUEEN);
            castleRights.remove(turn, CastleSide::KING);
        }
        else if (move.enPassant)
        {
            Square capturedPawnSq = backward(turn)(move.to).value();
            position.remove(capturedPawnSq);
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
                castleRights.remove(turn, CastleSide::QUEEN);
            if (move.from.file == 7)
                castleRights.remove(turn, CastleSide::KING);
        }
        if (turn == Color::Black)
            fullTurnCounter++;
        turn = oppositeColor(turn);
    }

    Move State::applyPGNMove(std::string_view pgnMove)
    {
        Move move = interpretPGNMove(pgnMove, position, turn);
        applyMove(move);
        return move;
    }

    Move State::applyUCIMove(std::string_view uciMove)
    {
        Move move = interpretUCIMove(uciMove, *this);
        applyMove(move);
        return move;
    }

    Move interpretPGNMove(std::string_view pgnMove,
                          const Position &previousPos,
                          Color turn)
    {
        Move move;

        move.piece.color = turn;
        move.piece.type = PGNPieceType.at(pgnMove[0]);

        std::cmatch match;
        std::regex move_string_regex(R"(([KQRBN])?([a-h]?[1-8]?)(x)?([a-h][1-8])(?:=([QRBN]))?([+#])?([!?]{1,2})?)");
        std::regex castle_regex(R"(O-O(-O)?([+#])?([!?]{1,2})?)");

        if (std::regex_match(pgnMove.data(), match, move_string_regex))
        {
            move.to = Square(match[4].str());
            move.from = computeFromSquare(move.to, move.piece, previousPos, match[2].str());

            if (match[3].matched)
            {
                if (move.piece.type == PieceType::Pawn && !previousPos.get(move.to))
                {
                    move.capture = PieceType::Pawn;
                    move.enPassant = true;
                }
                else
                    move.capture = previousPos.get(move.to).value().type;
            }

            if (match[5].matched)
                move.promotion = PGNPieceType.at(match[5].str()[0]);
        }
        else if (std::regex_match(pgnMove.data(), match, castle_regex))
        {
            move.castle = (match[1].str().empty()) ? CastleSide::KING : CastleSide::QUEEN;
            move.from.rank = homeRank(turn);
            move.to.rank = move.from.rank;
            move.from.file = 4;
            move.to.file = kingToFile(move.castle.value());
        }
        else
        {
            throw std::runtime_error("Invalid PGN move string");
        }

        return move;
    }

    Square computeFromSquare(Square to, Piece piece, const Position &pos, std::string_view partialFrom)
    {
        if (partialFrom.size() == 2)
            return Square(partialFrom);

        Square from{};
        bool singleSpace{true};
        std::vector<Offset> offsets{};
        offsets.reserve(8);
        std::vector<Square> candidates{};
        candidates.reserve(8);
        std::optional<Square> maybeSq{std::nullopt};
        switch (piece.type)
        {
        case PieceType::Pawn:
            singleSpace = true;
            for (const auto &offset : pawnOffsets)
                offsets.push_back(offset);
            /* code */
            break;

        case PieceType::Knight:
            singleSpace = true;
            for (const auto &offset : knightOffsets)
                offsets.push_back(offset);
            break;

        case PieceType::Bishop:
            singleSpace = false;
            for (const auto &offset : bishopOffsets)
                offsets.push_back(offset);
            break;

        case PieceType::Rook:
            singleSpace = false;
            for (const auto &offset : rookOffsets)
                offsets.push_back(offset);
            break;

        case PieceType::Queen:
            singleSpace = false;
            for (const auto &offset : queenKingOffsets)
                offsets.push_back(offset);
            break;

        case PieceType::King:
            singleSpace = true;
            for (const auto &offset : queenKingOffsets)
                offsets.push_back(offset);
            break;

        default:
            std::unreachable();
        }

        if (singleSpace)
        {
            for (const auto &offset : offsets)
            {
                maybeSq = offset(to);
                if (maybeSq &&
                    pos.get(maybeSq.value())
                            .value_or(Piece{Color::White, PieceType::Queen})
                            .type == piece.type)
                    candidates.push_back(maybeSq.value());
            }
        }
        else
        {
            for (const auto &offset : offsets)
            {
                for (const auto &sq : pos.getPath(to, offset))
                {
                    if (pos.get(sq))
                    {
                        if (pos.get(sq).value().type == piece.type)
                            candidates.push_back(sq);
                        break;
                    }
                }
            }
        }

        if (candidates.size() == 0)
            throw std::runtime_error("Could not identify which square the piece moved from");
        if (candidates.size() == 1)
            return candidates.front();
        if (partialFrom.size() == 0)
            throw std::runtime_error("Could not identify which square the piece moved from");

        bool matchRank;
        if ('1' <= partialFrom[0] && partialFrom[0] <= '8')
            matchRank = true;
        else if ('a' <= partialFrom[0] && partialFrom[0] <= 'h')
            matchRank = false;
        else
            throw std::runtime_error("Could not identify which square the piece moved from");

        std::string testString{};
        if (matchRank)
        {
            testString += "a";
            testString += partialFrom;
            Square test{testString};
            auto matchPartial = [&](Square sq)
            { return test.rank == sq.rank; };
            for (const auto &candidate : candidates)
                if (matchPartial(candidate))
                    return candidate;
            throw std::runtime_error("Could not identify which square the piece moved from");
        }
        else
        {
            testString += partialFrom;
            testString += "1";
            Square test{testString};
            auto matchPartial = [&](Square sq)
            { return test.file == sq.file; };
            for (const auto &candidate : candidates)
                if (matchPartial(candidate))
                    return candidate;
            throw std::runtime_error("Could not identify which square the piece moved from");
        }
    }

    Move interpretUCIMove(std::string_view uciMove, const State &state)
    {
    }
}