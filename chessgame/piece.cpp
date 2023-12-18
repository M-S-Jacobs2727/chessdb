#include "piece.h"

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

} // namespace ChessGame