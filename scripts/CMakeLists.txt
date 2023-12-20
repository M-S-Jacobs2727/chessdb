add_executable(converter converter.cpp)
target_include_directories(
    converter
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
    converter
    PRIVATE
    ChessFiles_static
)

add_executable(initdb db.cpp)
target_include_directories(
    initdb
    PUBLIC
    ${CMAKE_SOURCE_DIR}
)
target_link_libraries(
    initdb
    PRIVATE
    Database
)