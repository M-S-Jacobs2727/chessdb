#include <fstream>
#include <iostream>
#include <string>

#include "chessfiles/binaryFile.h"
#include "chessfiles/pgnFile.h"
#include "chessgame/game.h"

int main(int argc, char *argv[])
{
    if (argc != 3)
        return 1;

    std::string input_filename{argv[1]};
    std::string output_filename{argv[2]};

    std::ifstream input{input_filename};
    auto game = ChessGame::readPGN(input);
    input.close();

    std::ofstream output{output_filename};
    ChessGame::writeBinary(output, game);
    output.close();

    input.open(output_filename);
    auto game2 = ChessGame::readBinary(input);
    input.close();

    return 0;
}