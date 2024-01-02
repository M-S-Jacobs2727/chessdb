#include "internal/game/move.h"

#include <regex>
#include <unordered_map>

namespace ChessGame
{
    Move::Move(const std::string_view &str, Color color)
        : capturedPiece(Piece{oppositeColor(color), PieceType::None}),
          piece(Piece{color, readPGNPieceType(str)}),
          moveString(str), promotedPiece(Piece{color, PieceType::None})
    {
        std::regex move_string_regex(R"((O-O(-O)?|([KQRBN])?([a-h]?[1-8]?)(x)?([a-h][1-8])(?:=([QRBN]))?)([+#])?([!?]{1,2})?)");
        std::regex castle_regex(R"(O-O(-O)?([+#])?([!?]{1,2})?)");
        std::cmatch match;

        if (!std::regex_match(str.data(), match, move_string_regex))
            throw std::runtime_error("Invalid move string");

        std::string moveStr = match[1], checkStr = match[8], annoStr = match[9];
        check = !checkStr.empty();

        if (moveStr[0] == 'O')
        {
            from.file = 4;

            bool kingSide = !match[2].str().empty();
            to.file = kingSide ? 6 : 2;

            from.rank = (color == Color::White) ? 0 : 7;
            to.rank = (color == Color::White) ? 0 : 7;
        }
        else
        {
            partialFrom = match[3].str();
            if (partialFrom.size() == 2)
                from = Square(partialFrom);

            if (match[4].matched)
                capturedPiece.type = PieceType::Any;

            to = Square(match[5].str());

            auto promotionString = match[6].str();
            if (promotionString.size())
                promotedPiece.type = readPGNPieceType(promotionString);
        }
    }
} // namespace ChessGame
