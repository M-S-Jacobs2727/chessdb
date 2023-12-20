#include <iostream>

#include <pqxx/pqxx>

#include "database/games.h"

int main()
{
    try
    {
        pqxx::connection conn;
        pqxx::work work(conn);

        work.commit();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}