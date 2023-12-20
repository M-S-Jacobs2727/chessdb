cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

# Set the project name
project(ChessGame)

# Options
option(TEST "Enable testing" ON)

# Add subdirectories
add_subdirectory(chessgame)
add_subdirectory(chessfiles)
add_subdirectory(database)
if(TEST)
    add_subdirectory(test) # Add the test directory
endif()

add_subdirectory(scripts)
