add_executable(converter converter.cpp)
target_link_libraries(
    converter
    PRIVATE
    ChessGame_shared
    ChessFiles_shared
)
target_include_directories(
    converter
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)

add_executable(loadDatabase db.cpp)
target_include_directories(
    loadDatabase
    PUBLIC
    ${CMAKE_SOURCE_DIR}/pqxx/include/
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
    loadDatabase
    PRIVATE
    ChessGame_shared
    ChessFiles_shared
    ${CMAKE_SOURCE_DIR}/pqxx/lib/libpqxx.a
    pq
)