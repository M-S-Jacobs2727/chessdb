add_library(ChessGame OBJECT)

set_property(TARGET ChessGame PROPERTY POSITION_INDEPENDENT_CODE ON)
file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
target_sources(ChessGame PRIVATE ${SOURCES})
target_include_directories(ChessGame PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Create both shared and static libraries
add_library(ChessGame_shared SHARED $<TARGET_OBJECTS:ChessGame>)
add_library(ChessGame_static STATIC $<TARGET_OBJECTS:ChessGame>)
