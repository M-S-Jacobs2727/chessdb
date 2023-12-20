add_library(ChessFiles OBJECT)

set_property(TARGET ChessFiles PROPERTY POSITION_INDEPENDENT_CODE ON)
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(ChessFiles PRIVATE ${SOURCES})
target_include_directories(
    ChessFiles
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(ChessFiles PUBLIC ChessGame_static)

# Create both shared and static libraries
add_library(ChessFiles_shared SHARED $<TARGET_OBJECTS:ChessFiles>)
add_library(ChessFiles_static STATIC $<TARGET_OBJECTS:ChessFiles>)