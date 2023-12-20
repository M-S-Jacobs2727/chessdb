add_library(Database STATIC)

file(GLOB SOURCES "*.cpp")
target_sources(Database PRIVATE ${SOURCES})
target_include_directories(
    Database
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/pqxx/include/
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
    Database
    PUBLIC
    ${CMAKE_SOURCE_DIR}/pqxx/lib/libpqxx.a
    pq
    ChessGame
    ChessFiles
)
