#pragma once

#include <vector>

#include <pqxx/pqxx>

#include "internal/game/game.h"
#include "internal/game/move.h"

namespace ChessGame
{
    std::string_view assembleMovesBlob(const std::vector<Move> &moves);
    std::string_view insertGameCommand(const Game &game);
    bool insertGame(pqxx::connection &conn, const Game &game);
} // namespace ChessGame
