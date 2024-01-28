#include "internal/logic/castling.h"
#include <gtest/gtest.h>

namespace cstl = ChessGame::Castling;

TEST(CastlingTest, BasicAssertions)
{
    using ChessGame::Color, cstl::Side;

    cstl::Rights rights;
    ASSERT_TRUE(rights.get(Color::White, Side::QUEEN));
    ASSERT_TRUE(rights.get(Color::White, Side::KING));
    ASSERT_TRUE(rights.get(Color::Black, Side::QUEEN));
    ASSERT_TRUE(rights.get(Color::Black, Side::KING));
}
