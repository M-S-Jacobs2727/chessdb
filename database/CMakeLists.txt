# Add the library
add_library(Database STATIC)

# Add all .cpp files in the chessfiles directory
file(GLOB_RECURSE SOURCES "*.cpp")
target_sources(ChessFiles PRIVATE ${SOURCES})

target_include_directories(ChessFiles PUBLIC "./" PUBLIC "../chessgame")
