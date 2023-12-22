#pragma once

#include <istream>
#include <optional>
#include <string_view>
#include <vector>

#include <pqxx/pqxx>

#include "chessgame/board.h"
#include "chessgame/game.h"
#include "chessgame/move.h"
#include "chessgame/piece.h"
#include "chessgame/pieces.h"

namespace ChessGame
{
    class Database
    {
    public:
        Database() = default;
        ~Database();
        void insertPGN(std::istream &pgnIn);

        void findGamesByPosition(const Position &pieces);
        void findGamesByMoves(const std::vector<Move> &moves);
        void findGamesByOpening(std::string_view openingECO);
        void findGamesByPlayer(std::string_view player, std::optional<Color> = std::nullopt);
        void findGamesByDate();

        void removeGame(const Game &game);

    private:
        bool insertGame(const Game &game);
        bool insertMoves(const std::vector<Move> &moves);
        bool insertMove(const Move &move);
        bool insertPosition(const Board &board);

        std::string_view assembleMovesBlob(const std::vector<Move> &moves);
        std::string_view insertGameCommand(const Game &game);
        std::string_view insertMoveCommand(const Move &moves);
        std::string_view insertPositionCommand(const Position &pieces);

    private:
        pqxx::connection m_conn;
    };
} // namespace ChessGame