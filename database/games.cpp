#include "games.h"

#include <array>
#include <cstdint>
#include <format>
#include <sstream>
#include <string>

#include <pqxx/pqxx>

namespace ChessGame
{
    std::string_view assembleMovesBlob(const std::vector<Move> &moves)
    {
        std::string moveData;
        moveData.resize(moves.size() * 4);
        uint16_t m = 0;
        uint64_t count = 0;
        for (const auto &move : moves)
        {
            uint16_t m = static_cast<uint16_t>(move.piece.color);
            m <<= 3;
            m |= static_cast<uint16_t>(move.piece.type);
            m <<= 3;
            m |= move.from.file;
            m <<= 3;
            m |= move.from.rank;
            m <<= 3;
            m |= move.to.file;
            m <<= 3;
            m |= move.to.rank;

            count += 4;
            moveData[count - 1] = '0' + (m & 0x000F);
            m >>= 4;
            moveData[count - 2] = '0' + (m & 0x000F);
            m >>= 4;
            moveData[count - 3] = '0' + (m & 0x000F);
            m >>= 4;
            moveData[count - 4] = '0' + (m & 0x000F);
        }
        return moveData;
    }

    std::string_view insertGameCommand(const Game &game)
    {
        constexpr std::array<std::string_view, 4> results{"white", "black", "draw", "none"};
        constexpr std::array<std::string_view, 8> conditions{
            "none", "checkmate", "timeout", "resignation",
            "infraction", "stalemate", "agreement", "material"};

        std::ostringstream command{
            "INSERT INTO games (gameID, datetimeUTC, whiteUsername, blackUsername,"
            " whiteELO, blackELO, timeLimit, timeIncrement, winner, condition,"
            " halfMoves, ecoCode, movesBlob) VALUES "};

        auto timestamp = std::format("timestamp (0) '{:04d}{:02d}{:02d} {:02d}:{:02d}:{:02d}'",
                                     game.datetime.year, game.datetime.month, game.datetime.day,
                                     game.datetime.hour, game.datetime.minute, game.datetime.second);
        auto timeLimit = std::format("INTERVAL '{} seconds'", game.timeControl.initialTime);
        auto timeIncrement = std::format("INTERVAL '{} seconds'", game.timeControl.increment);
        auto result = results[static_cast<int>(game.result.type)];
        auto condition = conditions[static_cast<int>(game.result.reason)];
        auto movesBlob = std::format("bytea '\\x{}');", assembleMovesBlob(game.moves));

        command << "(DEFAULT, " << timestamp << ", " << game.whiteUsername << ", " << game.blackUsername << ", "
                << game.whiteELO << ", " << game.blackELO << ", " << timeLimit << ", " << timeIncrement << ", "
                << result << ", " << condition << ", " << game.moves.size() << ", " << game.ECOCode << ", "
                << movesBlob << ");";

        return command.str();
    }
} // namespace ChessGame
