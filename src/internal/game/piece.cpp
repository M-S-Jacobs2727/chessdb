#include "internal/game/piece.h"

#include "piece.h"
#include <array>
#include <stdexcept>
#include <string_view>
#include <unordered_map>

namespace ChessGame
{
    PieceType readPGNPieceType(const std::string_view &moveStr)
    {
        const static std::unordered_map<char, PieceType> pieceTypeMap = {
            {'O', PieceType::King},
            {'K', PieceType::King},
            {'Q', PieceType::Queen},
            {'R', PieceType::Rook},
            {'B', PieceType::Bishop},
            {'N', PieceType::Knight},
            {'a', PieceType::Pawn},
            {'b', PieceType::Pawn},
            {'c', PieceType::Pawn},
            {'d', PieceType::Pawn},
            {'e', PieceType::Pawn},
            {'f', PieceType::Pawn},
            {'g', PieceType::Pawn},
            {'h', PieceType::Pawn},
        };
        return pieceTypeMap.at(moveStr[0]);
    }

    std::string_view toPGN(const PieceType pt)
    {
        using namespace std::string_view_literals;
        const static std::array<std::string_view, 8> pieceTypeArr{
            ""sv, ""sv, "N"sv, "B"sv, "R"sv, "Q"sv, "K"sv, ""sv};
        return pieceTypeArr[static_cast<int>(pt)];
    }

    std::string_view toFEN(const PieceType pt, const Color color)
    {
        using namespace std::string_view_literals;
        const static std::array<std::string_view, 16> pieceTypeArr{
            ""sv, "P"sv, "N"sv, "B"sv, "R"sv, "Q"sv, "K"sv, ""sv, ""sv, "p"sv, "n"sv, "b"sv, "r"sv, "q"sv, "k"sv, ""sv};
        return pieceTypeArr[static_cast<int>(pt) + (color == Color::Black) ? 8 : 0];
    }

    std::optional<std::string_view> toStr(const PieceType pt)
    {
        using namespace std::string_view_literals;
        const static std::array<std::string_view, 8> pieceTypeArr{
            ""sv, "pawn"sv, "knight"sv, "bishop"sv, "rook"sv, "queen"sv, "king"sv, ""sv};
        auto p = pieceTypeArr[static_cast<int>(pt)];
        return p.empty() ? std::nullopt : std::make_optional(p);
    }
    constexpr std::string_view toStr(const Color color)
    {
        using namespace std::string_view_literals;
        return (color == Color::Black) ? "black"sv : "white"sv;
    }

} // namespace ChessGame