#pragma once

#include <cstdint>
#include <optional>
#include <string_view>
#include <variant>

namespace ChessGame
{
    using eval_t = std::variant<int, float>;
    struct Evaluation
    {
        eval_t value = 0.0f;

        Evaluation() = default;
        explicit Evaluation(eval_t eval) : value(eval) {}
        explicit Evaluation(std::string_view evalString);
    };

    struct Evaluation2
    {
        enum class Unit
        {
            CP,
            MATE
        };
        Unit unit = Unit::CP;
        int32_t value = 0;
    };
}