#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "tokens.h"
#include "visitor/print_visitor.h"
#include <iostream>

void run(ProgramNode *program) {
    auto *interpreter_visitor = new InterpreterVisitor();
    program->evaluate(interpreter_visitor, nullptr);
    delete interpreter_visitor;
}

void built_and_run(const std::string &path) {
    std::string code = Reader::read_file(path);
    std::vector<Token> tokens = Lexer::parse_tokens(code);

    ProgramNode *program = Parser::parse_program(tokens);
    auto *semantic_analysis_visitor = new SemanticAnalysisVisitor();
    program->analyze(semantic_analysis_visitor, nullptr);
    delete semantic_analysis_visitor;

    Error::print_build_status();
    if (Error::should_quit()) {
        delete program;
        exit(1);
    } else {
        run(program);
    }

    delete program;
}

void print_usage() {
    std::printf("Usage: sscript <path>\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && Reader::file_exists(argv[1])) {
        built_and_run(argv[1]);
    } else {
        print_usage();
    }
}
