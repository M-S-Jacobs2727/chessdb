#pragma once

#include <istream>

#include <pqxx/pqxx>

namespace ChessGame
{
    void insertGame(pqxx::connection &conn, const Game &game);
} // namespace ChessGame