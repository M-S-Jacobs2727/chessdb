# FILEPATH: /home/jacobs/code/chessdb/test/CMakeLists.txt
# BEGIN: abpxx6d04wxr

find_package(GTest REQUIRED)
enable_testing()

file(GLOB TEST_SOURCES "*.cpp")
foreach(TEST_SOURCE ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    add_executable(${TEST_NAME} ${TEST_SOURCE})

    target_link_libraries(
        ${TEST_NAME} PRIVATE
        GTest::gtest_main 
        ChessGame_shared ChessFiles_shared
    )
    target_include_directories(${TEST_NAME} PUBLIC "../chessgame" PUBLIC "../chessfiles")
    include(GoogleTest)

    gtest_discover_tests(${TEST_NAME})

endforeach()
# END: abpxx6d04wxr
