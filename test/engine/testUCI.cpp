#include "engine/uci.h"

#include <gtest/gtest.h>

TEST(UCITest, Startup)
{
    JChess::UCI uci{"stockfish"};
}