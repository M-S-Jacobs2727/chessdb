#include "database/blobs.h"
#include "blobs.h"

namespace JChess
{
    template <class T, class U>
    constexpr void writeToBits(T &bits, U value, uint8_t shift)
    {
        bits <<= shift;
        bits |= static_cast<T>(value);
    }

    template <class T, class U>
    constexpr T readFromBits(const T &bits, U mask, uint8_t shift)
    {
        T value = (bits & static_cast<T>(mask)) >> shift;
        // bits >>= shift;
        return value;
    }

    blob movesToBlob(const std::vector<Move> &moves)
    {
        blob movesBlob;
        movesBlob.reserve(moves.size() * 2 + 1);

        for (const auto &move : moves)
        {
            std::byte bits{0};
            writeToBits(bits, move.from.file, 3);
            writeToBits(bits, move.from.rank, 3);
            movesBlob.push_back(bits);

            bits <<= 8;
            writeToBits(bits, move.to.file, 3);
            writeToBits(bits, move.to.rank, 3);
            movesBlob.push_back(bits);
        }

        return movesBlob;
    }

    std::vector<Move> blobToMoves(const blob &movesBlob)
    {
        std::vector<Move> moves;
        moves.reserve(movesBlob.size() / 2);

        for (size_t i = 0; i < movesBlob.size() / 2; ++i)
        {
            moves.emplace_back();
            auto &move = moves.back();

            const auto &fromBits = movesBlob[2 * i];
            move.from.file = static_cast<uint8_t>(readFromBits(fromBits, 0x38u, 3));
            move.from.rank = static_cast<uint8_t>(readFromBits(fromBits, 0x07u, 0));

            const auto &toBits = movesBlob[2 * i + 1];
            move.to.file = static_cast<uint8_t>(readFromBits(toBits, 0x38u, 3));
            move.to.rank = static_cast<uint8_t>(readFromBits(toBits, 0x07u, 0));
        }

        return moves;
    }

    blob positionToBlob(const Board &board)
    {
        blob blob;
        blob.reserve(32);
        for (size_t i = 0; i < 32; ++i)
        {
            std::byte bits{0};
            writeToBits(bits, board[2 * i].color, 1);
            writeToBits(bits, board[2 * i].type, 3);
            writeToBits(bits, board[2 * i + 1].color, 1);
            writeToBits(bits, board[2 * i + 1].type, 3);

            blob.push_back(bits);
        }
        return blob;
    }

    Board blobToPosition(const blob &posBlob)
    {
        Board pos;
        for (size_t i = 0; i < 32; ++i)
        {
            const auto &bits = posBlob[i];
            pos[2 * i].color = static_cast<Color>(readFromBits(bits, 0x80u, 7));
            pos[2 * i].type = static_cast<PieceType>(readFromBits(bits, 0x70u, 4));
            pos[2 * i + 1].color = static_cast<Color>(readFromBits(bits, 0x08u, 3));
            pos[2 * i + 1].type = static_cast<PieceType>(readFromBits(bits, 0x07u, 0));
        }
        return pos;
    }

} // namespace ChessGame
