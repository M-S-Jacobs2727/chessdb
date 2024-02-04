#pragma once

#include <array>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include "core/castling.h"
#include "core/color.h"
#include "core/offset.h"
#include "core/piece.h"
#include "core/square.h"

#include "formats/fen.h"

namespace ChessGame
{
    using Occupant = std::optional<Piece>;

    class Board
    {
    public:
        explicit Board(std::string_view fenString = FEN::startpos);
        Occupant get(Square square) const;
        Occupant put(Square square, Piece piece);
        Occupant remove(Square square);
        Square kingSquare(Color color) const;

        std::string toFen() const;

        /*
         * Returns a (possibly empty) vector of `Square`s along a given direction,
         * up to and optionally including the first piece found.
         */
        std::vector<Square> getPath(Square fromSquare, Offset direction, bool includePiece) const;
        constexpr const std::array<Occupant, 64> &eachOccupant() const;

        static constexpr std::array<Square, 64> eachSquare();

        static constexpr bool valid(Square square);
        static constexpr size_t squareToIdx(Square square);
        static constexpr Square idxToSquare(size_t idx);
        static constexpr int homeRank(Color color);

        static constexpr int rookFromFile(Castling::Side side);
        static constexpr int rookToFile(Castling::Side side);
        static constexpr int kingToFile(Castling::Side side);
        static constexpr Square rookFromSquare(Color color, Castling::Side side);
        static constexpr Square rookToSquare(Color color, Castling::Side side);
        static constexpr Square kingToSquare(Color color, Castling::Side side);

    private:
        std::array<Occupant, 64> m_arr;
    };
} // namespace ChessGame
