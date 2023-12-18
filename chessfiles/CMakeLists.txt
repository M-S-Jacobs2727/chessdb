# Add the library
add_library(ChessFiles OBJECT)
set_property(TARGET ChessFiles PROPERTY POSITION_INDEPENDENT_CODE ON)

# Add all .cpp files in the chessfiles directory
file(GLOB_RECURSE SOURCES "*.cpp")
target_sources(ChessFiles PRIVATE ${SOURCES})

target_include_directories(ChessFiles PUBLIC "./" PUBLIC "../chessgame")

# Link the ChessGame library with the ChessFiles library
target_link_libraries(ChessFiles PRIVATE ChessGame)

add_library(ChessFiles_shared SHARED)
target_link_libraries(ChessFiles_shared ChessFiles)
add_library(ChessFiles_static STATIC)
target_link_libraries(ChessFiles_static ChessFiles)