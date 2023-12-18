# Add the library
add_library(ChessGame OBJECT)

# Add all .cpp files in the chessgame directory
set_property(TARGET ChessGame PROPERTY POSITION_INDEPENDENT_CODE ON)
file(GLOB_RECURSE SOURCES "*.cpp")
target_sources(ChessGame PRIVATE ${SOURCES})
target_include_directories(ChessGame PUBLIC "./")

add_library(ChessGame_shared SHARED)
target_link_libraries(ChessGame_shared ChessGame)
add_library(ChessGame_static STATIC)
target_link_libraries(ChessGame_static ChessGame)
