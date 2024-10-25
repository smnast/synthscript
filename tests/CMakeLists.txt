set (TEST_SOURCES
    test_main.cpp
    test_error_manager.cpp
    test_reader.cpp
    test_lexer.cpp
    test_parser.cpp
    visitor/test_semantic_analysis_visitor.cpp
    visitor/test_interpreter_visitor.cpp
    utils/stream_redirect.cpp
    utils/test_stream_redirect.cpp
    utils/temp_file.cpp
    utils/test_temp_file.cpp
    utils/shortcuts.cpp
)

add_executable(tests ${TEST_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/include ${CMAKE_SOURCE_DIR}/tests)

target_link_libraries(tests PRIVATE SynthScriptLib doctest)

add_test(NAME tests COMMAND tests)
