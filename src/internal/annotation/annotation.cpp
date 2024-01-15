#include "annotation.h"

#include <array>
#include <unordered_map>

namespace ChessGame
{
    Annotation readAnnotation(const std::string_view &str)
    {
        const static std::unordered_map<std::string_view, Annotation> annoMap = {
            {"", Annotation::None},
            {"!", Annotation::Good},
            {"?", Annotation::Mistake},
            {"!!", Annotation::Brilliant},
            {"??", Annotation::Blunder},
            {"!?", Annotation::Interesting},
            {"?!", Annotation::Dubious},
        };
        return annoMap.at(str);
    }

    std::optional<std::string_view> toPGN(const Annotation annotation)
    {
        using namespace std::string_view_literals;
        std::array<std::string_view, 8> annoArr{
            ""sv,
            "!"sv,
            "?"sv,
            "!!"sv,
            "??"sv,
            "!?"sv,
            "?!"sv,
            ""sv};
        auto a = annoArr[static_cast<int>(annotation)];
        return a.empty() ? std::nullopt : std::make_optional(a);
    }

    std::optional<std::string_view> toStr(const std::optional<Annotation> annotation)
    {
        if (!annotation.has_value())
            return std::nullopt;

        using namespace std::string_view_literals;
        std::array<std::string_view, 8> annoArr{
            ""sv,
            "mistake"sv,
            "good"sv,
            "blunder"sv,
            "brilliant"sv,
            "interesting"sv,
            "dubious"sv,
            ""sv};
        auto a = annoArr[static_cast<int>(annotation.value())];
        return a.empty() ? std::nullopt : std::make_optional(a);
    }

} // namespace ChessGame
