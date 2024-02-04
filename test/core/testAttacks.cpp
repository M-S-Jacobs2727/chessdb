#include "core/attacks.h"
#include <gtest/gtest.h>

using JChess::Attacks, JChess::Board;

TEST(AttacksTest, BasicAssertions)
{
    Board pos{"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR"};
    auto pos_p = std::make_shared<Board>(pos);
    Attacks att{pos_p};

    {
        int nums[] = {0, 1, 1, 1, 1, 1, 1, 0};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 0}, JChess::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "1.";
            num = att.numAttackers({i, 0}, JChess::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "1.";
            num = att.numAttackers({i, 7}, JChess::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "8.";
            num = att.numAttackers({i, 7}, JChess::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "8.";
        }
    }
    {
        int nums[] = {1, 1, 1, 4, 4, 1, 1, 1};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 1}, JChess::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "2.";
            num = att.numAttackers({i, 1}, JChess::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "2.";
            num = att.numAttackers({i, 6}, JChess::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "7.";
            num = att.numAttackers({i, 6}, JChess::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "7.";
        }
    }
    {
        int nums[] = {2, 2, 3, 2, 2, 3, 2, 2};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 2}, JChess::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "3.";
            num = att.numAttackers({i, 2}, JChess::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "3.";
            num = att.numAttackers({i, 5}, JChess::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "6.";
            num = att.numAttackers({i, 5}, JChess::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers to " << (char)('a' + i) << "6.";
        }
    }
    for (size_t i = 0; i < 8; ++i)
    {
        auto num = att.numAttackers({i, 3}, JChess::Color::White);
        EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "4.";
        num = att.numAttackers({i, 3}, JChess::Color::Black);
        EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "4.";
        num = att.numAttackers({i, 4}, JChess::Color::White);
        EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "5.";
        num = att.numAttackers({i, 4}, JChess::Color::Black);
        EXPECT_EQ(0, num) << "Incorrect number of attackers to " << (char)('a' + i) << "5.";
    }
}