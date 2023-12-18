#include <iostream>

#include <pqxx/pqxx>

void main()
{
    try
    {
        pqxx::connection conn;
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }
}