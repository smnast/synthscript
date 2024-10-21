set (TEST_SOURCES
    test_main.cpp
    test_error_manager.cpp
    utils/test_cout_redirect.cpp
)

add_executable(tests ${TEST_SOURCES})

include_directories(${CMAKE_SOURCE_DIR}/include)

target_link_libraries(tests PRIVATE SynthScriptLib doctest)

add_test(NAME tests COMMAND tests)
