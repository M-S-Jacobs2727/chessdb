#include "internal/logic/attacks.h"
#include <gtest/gtest.h>

using ChessGame::Attacks, ChessGame::Position;

TEST(AttacksTest, BasicAssertions)
{
    Position pos{""};
    auto pos_p = std::make_shared<Position>(pos);
    Attacks att{pos_p};

    {
        int nums[] = {0, 1, 1, 1, 1, 1, 1, 0};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 0}, ChessGame::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 1.";
            num = att.numAttackers({i, 0}, ChessGame::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 1.";
            num = att.numAttackers({i, 7}, ChessGame::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 8.";
            num = att.numAttackers({i, 7}, ChessGame::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 8.";
        }
    }
    {
        int nums[] = {1, 1, 1, 4, 4, 1, 1, 1};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 1}, ChessGame::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 2.";
            num = att.numAttackers({i, 1}, ChessGame::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 2.";
            num = att.numAttackers({i, 6}, ChessGame::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 7.";
            num = att.numAttackers({i, 6}, ChessGame::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 7.";
        }
    }
    {
        int nums[] = {2, 2, 3, 2, 2, 3, 2, 2};

        for (size_t i = 0; i < 8; ++i)
        {
            auto num = att.numAttackers({i, 2}, ChessGame::Color::White);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 3.";
            num = att.numAttackers({i, 2}, ChessGame::Color::Black);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 3.";
            num = att.numAttackers({i, 5}, ChessGame::Color::White);
            EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 6.";
            num = att.numAttackers({i, 5}, ChessGame::Color::Black);
            EXPECT_EQ(nums[i], num) << "Incorrect number of attackers in rank 6.";
        }
    }
    for (size_t i = 0; i < 8; ++i)
    {
        auto num = att.numAttackers({i, 3}, ChessGame::Color::White);
        EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 4.";
        auto num = att.numAttackers({i, 3}, ChessGame::Color::Black);
        EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 4.";
        auto num = att.numAttackers({i, 4}, ChessGame::Color::White);
        EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 5.";
        auto num = att.numAttackers({i, 4}, ChessGame::Color::Black);
        EXPECT_EQ(0, num) << "Incorrect number of attackers in rank 5.";
    }
}