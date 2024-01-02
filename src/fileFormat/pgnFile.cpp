#include <bitset>
#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>
#include <optional>
#include <regex>
#include <sstream>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

#include "internal/evaluation.h"
#include "internal/game/game.h"
#include "internal/time/clockTime.h"

#include "fileFormat/pgnFile.h"

namespace ChessGame
{
    void readPGNHeader(std::istream &input, Game &game)
    {
        std::unordered_map<std::string, GameResult::Type> resultTypeMap{
            {"1-0", GameResult::Type::WhiteWins},
            {"0-1", GameResult::Type::BlackWins},
            {"1/2-1/2", GameResult::Type::Draw},
            {"*", GameResult::Type::Other}};

        std::unordered_map<std::string, GameResult::Reason> resultReasonMap{
            {"Normal", GameResult::Reason::None},
            {"Time forfeit", GameResult::Reason::Timeout},
            {"Rules infraction", GameResult::Reason::RulesInfraction},
            {"Abandoned", GameResult::Reason::Checkmate},
            {"Unterminated", GameResult::Reason::Checkmate}};

        std::string line, dateString, timeString;

        while (std::getline(input, line) && !line.empty())
        {
            // Remove the leading and trailing square brackets.
            std::istringstream iss(line.substr(1, line.size() - 2));
            std::string tag;
            std::getline(iss, tag, ' ');
            std::string value;
            std::getline(iss, value, '"');
            std::getline(iss, value, '"');

            // Store the value in the appropriate field of the header.
            // This is left as an exercise for the reader.
            if (tag == "White")
                game.whiteUsername = value;
            else if (tag == "Black")
                game.blackUsername = value;
            else if (tag == "Result")
                game.result.type = resultTypeMap[value];
            else if (tag == "UTCDate")
                dateString = value;
            else if (tag == "UTCTime")
                timeString = value;
            else if (tag == "WhiteElo")
                game.whiteELO = std::stoi(value);
            else if (tag == "BlackElo")
                game.blackELO = std::stoi(value);
            else if (tag == "ECO")
                game.ECOCode = value;
            else if (tag == "TimeControl")
                game.timeControl = TimeControl(value);
            else if (tag == "Termination")
                game.result.reason = resultReasonMap[value];
        }

        game.datetime = Datetime(dateString, timeString);
    }

    void readPGNMoves(std::istream &input, Game &game)
    {
        std::string line;
        std::getline(input, line);

        std::regex move_regex(R"(\d+\.{1,3}\s+(\S+)\s+(?:\{\s*(?:\[%eval\s+(\S+)\])?\s*(?:\[%clk\s+(\S+)\])?\s*\})?)");
        auto move_begin = std::sregex_iterator(line.begin(), line.end(), move_regex);
        auto move_end = std::sregex_iterator();

        std::unordered_map<std::string, PieceType> pieceTypeMap{
            {"K", PieceType::King},
            {"Q", PieceType::Queen},
            {"R", PieceType::Rook},
            {"B", PieceType::Bishop},
            {"N", PieceType::Knight},
            {"", PieceType::Pawn}};
        std::unordered_map<std::string, Annotation> annotationMap{
            {"", Annotation::None},
            {"!", Annotation::Good},
            {"!!", Annotation::Brilliant},
            {"?", Annotation::Mistake},
            {"??", Annotation::Blunder},
            {"!?", Annotation::Interesting},
            {"?!", Annotation::Dubious}};

        Color currentColor = Color::White;
        game.evaluations.emplace();
        game.clocks.emplace();
        for (auto i = move_begin; i != move_end; ++i)
        {
            game.moves.emplace_back();
            auto &move = game.moves.back();

            move.capturedPiece = Piece{Color::White, PieceType::None};

            std::smatch match = *i;
            std::string move_string = match[1].str();

            auto evalString = match[2].str();
            if (!evalString.empty())
                game.evaluations.value().emplace_back(evalString);

            auto clockString = match[3].str();
            if (!clockString.empty())
                game.clocks.value().emplace_back(clockString);

            move.piece.color = currentColor;
            currentColor = oppositeColor(currentColor);

            auto idx = move_string.find_first_of("!?");
            std::string annotation_string = (idx == std::string::npos) ? "" : move_string.substr(idx);
            move.moveString = move_string.substr(0, idx);

            move.check = (move.moveString.back() == '#') || (move.moveString.back() == '+');

            std::regex move_string_regex(R"(([KQRBN])?([a-h]?[1-8]?)(x)?([a-h][1-8])(?:=([QRBN]))?([+#])?)");
            std::regex castle_regex(R"(O-O(-O)?([+#])?)");
            std::regex annotation_regex(R"(([!?]{1,2})?)");

            if (std::regex_match(move.moveString, match, move_string_regex))
            {
                move.piece.type = pieceTypeMap[match[1].str()];
                if (match[2].str().size() == 2)
                    move.from = Square(match[2].str());

                if (match[3].matched)
                    move.capturedPiece = Piece{Color::White, PieceType::Any};

                move.to = Square(match[4].str());
            }
            else if (std::regex_match(move.moveString, match, castle_regex))
            {
                move.piece.type = PieceType::King;
                if (currentColor == Color::White)
                {
                    move.from = Square("e1");
                    move.to = Square(match[1].str().empty() ? "g1" : "c1");
                }
                else
                {
                    move.from = Square("e8");
                    move.to = Square(match[1].str().empty() ? "g8" : "c8");
                }
            }
            else
            {
                throw std::runtime_error("Invalid move string");
            }

            // if (std::regex_match(annotation_string, match, annotation_regex))
            //     move.annotation = annotationMap[match[2].str()];
            // else
            //     throw std::runtime_error("Invalid extra string");
        }

        if (game.evaluations.value().size() < game.moves.size())
            game.evaluations.reset();
        if (game.clocks.value().size() < game.moves.size())
            game.clocks.reset();
    }

    Game readPGN(std::istream &input)
    {
        Game game;
        readPGNHeader(input, game);
        readPGNMoves(input, game);

        State board;
        game.states.reserve(game.moves.size());
        for (auto &move : game.moves)
        {
            board.applyMove(move);
            game.states.push_back(board); // intentional copy
        }

        return game;
    }
}