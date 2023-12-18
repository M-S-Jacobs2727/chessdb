cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)

# Set the project name
project(ChessGame)

# Options
option(TEST "Enable testing" ON)

# Add subdirectories
add_subdirectory(chessgame)
add_subdirectory(chessfiles)
if(TEST)
    add_subdirectory(test) # Add the test directory
endif()

add_executable(converter converter.cpp)
target_link_libraries(converter PRIVATE ChessGame_shared ChessFiles_shared)
target_include_directories(converter PUBLIC chessgame chessfiles)
