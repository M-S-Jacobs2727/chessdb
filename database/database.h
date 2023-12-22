#pragma once

#include <istream>
#include <optional>
#include <string_view>
#include <vector>

#include <pqxx/pqxx>

#include "internal/game/game.h"
#include "internal/game/move.h"
#include "internal/game/piece.h"
#include "internal/game/position.h"

namespace ChessGame
{
    class Database
    {
    public:
        Database() = default;
        ~Database();
        void insertPGN(std::istream &pgnIn);

        void findGamesByPosition(const Position &position);
        void findGamesByMoves(const std::vector<Move> &moves);
        void findGamesByOpening(std::string_view openingECO);
        void findGamesByPlayer(std::string_view player, std::optional<Color> = std::nullopt);
        void findGamesByDate();

        void removeGame(const Game &game);

    private:
        bool insertGame(const Game &game);
        bool insertMoves(const std::vector<Move> &moves);
        bool insertMove(const Move &move);
        bool insertPosition(const Position &position);

        std::string_view assembleMovesBlob(const std::vector<Move> &moves);
        std::string_view insertGameCommand(const Game &game);
        std::string_view insertMoveCommand(const Move &moves);
        std::string_view insertPositionCommand(const Position &position);

    private:
        pqxx::connection m_conn;
    };
} // namespace ChessGame