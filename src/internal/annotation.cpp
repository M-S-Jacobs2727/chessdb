#include "annotation.h"

#include <unordered_map>

namespace ChessGame
{
    Annotation readAnnotation(const std::string_view &str)
    {
        const static std::unordered_map<std::string_view, Annotation> annoMap = {
            {"", Annotation::None},
            {"?", Annotation::Mistake},
            {"!", Annotation::Good},
            {"!!", Annotation::Brilliant},
            {"??", Annotation::Blunder},
            {"?!", Annotation::Dubious},
            {"!?", Annotation::Interesting},
        };
        return annoMap.at(str);
    }

} // namespace ChessGame
