#pragma once

#include <string>
#include <string_view>
#include <unordered_map>

namespace JChess
{
    enum class Annotation
    {
        None,
        Good,
        Mistake,
        Brilliant,
        Blunder,
        Interesting,
        Dubious,
    };

    namespace Annotations
    {
        using namespace std::string_view_literals;
        const inline std::unordered_map<std::string_view, Annotation> fromPGN = {
            {""sv, Annotation::None},
            {"!"sv, Annotation::Good},
            {"?"sv, Annotation::Mistake},
            {"!!"sv, Annotation::Brilliant},
            {"??"sv, Annotation::Blunder},
            {"!?"sv, Annotation::Interesting},
            {"?!"sv, Annotation::Dubious}};

        using namespace std::string_literals;
        const inline std::unordered_map<Annotation, std::string> toPGN = {
            {Annotation::None, ""s},
            {Annotation::Good, "!"s},
            {Annotation::Mistake, "?"s},
            {Annotation::Brilliant, "!!"s},
            {Annotation::Blunder, "??"s},
            {Annotation::Interesting, "!?"s},
            {Annotation::Dubious, "?!"s}};

        const inline std::unordered_map<Annotation, std::string> toWord = {
            {Annotation::None, ""s},
            {Annotation::Good, "good"s},
            {Annotation::Mistake, "mistake"s},
            {Annotation::Brilliant, "brilliant"s},
            {Annotation::Blunder, "blunder"s},
            {Annotation::Interesting, "interesting"s},
            {Annotation::Dubious, "dubious"s}};
    } // namespace Annotations
} // namespace JChess
