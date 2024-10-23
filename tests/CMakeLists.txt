set (TEST_SOURCES
    test_main.cpp
    test_error_manager.cpp
    test_reader.cpp
    test_lexer.cpp
    test_parser.cpp
    utils/cout_redirect.cpp
    utils/test_cout_redirect.cpp
    utils/temp_file.cpp
    utils/test_temp_file.cpp
)

add_executable(tests ${TEST_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(tests PRIVATE SynthScriptLib doctest)

add_test(NAME tests COMMAND tests)
