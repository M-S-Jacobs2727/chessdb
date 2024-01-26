#include "engine/uci.h"

#include <gtest/gtest.h>

TEST(UCITest, Startup)
{
    ChessGame::UCI uci{"stockfish"};
}