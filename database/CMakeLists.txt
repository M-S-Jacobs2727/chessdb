add_library(Database STATIC)

find_library(pqxx NAMES pqxx PATHS ENV LD_LIBRARY_PATH REQUIRED)
find_library(pq NAMES pq PATHS ENV LD_LIBRARY_PATH REQUIRED)

file(GLOB SOURCES "*.cpp")
target_sources(Database PRIVATE ${SOURCES})
target_include_directories(
    Database
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
    Database
    PUBLIC
    ChessFiles_static
    PRIVATE
    pq
    pqxx
)
