set(SOURCES
    main.cpp
    Reader.cpp
    Lexer.cpp
    Error.cpp
    Parser.cpp
    Visitor/PrintVisitor.cpp
    Symbol/SymbolTable.cpp
    Visitor/SemanticAnalysisVisitor.cpp
    Visitor/InterpreterVisitor.cpp
    Object/FunctionObject.cpp
    Object/IntObject.cpp
    Object/FloatObject.cpp
    Object/StringObject.cpp
    Object/ArrayObject.cpp
    Object/BoolObject.cpp
    Object/VoidObject.cpp
    BuiltinFunctions.cpp
)

add_executable(SynthScript ${SOURCES})
set_target_properties(SynthScript PROPERTIES
    OUTPUT_NAME "sscript"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

include_directories(${CMAKE_SOURCE_DIR}/include)
