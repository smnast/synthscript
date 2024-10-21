set(LIBRARY_SOURCES
    reader.cpp
    lexer.cpp
    parser.cpp
    error_manager.cpp
    visitor/print_visitor.cpp
    symbol/symbol.cpp
    symbol/symbol_table.cpp
    types/types.cpp
    visitor/semantic_analysis_visitor.cpp
    visitor/interpreter_visitor.cpp
    object/function_object.cpp
    object/int_object.cpp
    object/float_object.cpp
    object/string_object.cpp
    object/array_object.cpp
    object/bool_object.cpp
    object/void_object.cpp
    built_in_functions.cpp
    operators.cpp
)

add_library(SynthScriptLib ${LIBRARY_SOURCES})

add_executable(SynthScript main.cpp)
target_link_libraries(SynthScript PRIVATE SynthScriptLib)

set_target_properties(SynthScript PROPERTIES
    OUTPUT_NAME "sscript"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

include_directories(${CMAKE_SOURCE_DIR}/include)
