#include "tokens.h"
#include "reader.h"
#include "lexer.h"
#include "parser.h"
#include "visitor/print_visitor.h"
#include <iostream>

void run(ProgramNode *program) {
    auto *interpreterVisitor = new InterpreterVisitor();
    program->evaluate(interpreterVisitor, nullptr);
    delete interpreterVisitor;
}

void buildAndRun(const std::string &path) {
    std::string code = Reader::read_file(path);
    std::vector<Token> tokens = Lexer::parse_tokens(code);

    ProgramNode *program = Parser::parse_program(tokens);
    auto *semanticAnalysisVisitor = new SemanticAnalysisVisitor();
    program->analyze(semanticAnalysisVisitor, nullptr);
    delete semanticAnalysisVisitor;

    Error::print_build_status();
    if (Error::should_quit()) {
        delete program;
        exit(1);
    } else {
        run(program);
    }

    delete program;
}

void printUsage() {
    std::printf("Usage: sscript <path>\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && Reader::file_exists(argv[1])) {
        buildAndRun(argv[1]);
    } else {
        printUsage();
    }
}
