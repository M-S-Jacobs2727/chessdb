#pragma once

#include <istream>

#include <pqxx/pqxx>

namespace JChess
{
    void insertGame(pqxx::connection &conn, const Game &game);
} // namespace JChess