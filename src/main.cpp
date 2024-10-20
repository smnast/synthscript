#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "tokens.h"
#include "visitor/print_visitor.h"
#include <iostream>

void build_and_run(const std::string &path);
void run(ProgramNode *program);
void print_usage();

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *file_path = argv[1];
        if (!Reader::file_exists(file_path)) {
            std::cout << "File not found: " << file_path << std::endl;
        } else {
            build_and_run(file_path);
        }
    } else {
        print_usage();
    }
}

void build_and_run(const std::string &path) {
    std::cout << "Building program..." << std::endl;

    // Lexical analysis
    std::string code = Reader::read_file(path);
    std::vector<Token> tokens = Lexer::parse_tokens(code);

    // Syntax analysis
    ProgramNode *program = Parser::parse_program(tokens);

    // Print the AST
    // auto *print_visitor = new PrintVisitor();
    // program->accept(print_visitor, 0);
    // delete print_visitor;

    // Semantic analysis
    auto *semantic_analysis_visitor = new SemanticAnalysisVisitor();
    program->analyze(semantic_analysis_visitor, nullptr);
    delete semantic_analysis_visitor;

    // Print build status
    std::string build_status =
        Error::get_status() == Error::BuildStatus::SUCCESS ? "SUCCESS" : "FAILURE";
    std::cout << "Build status:\t" << build_status << std::endl;
    std::cout << "Error count:\t" << Error::get_error_count() << std::endl;

    if (Error::get_status() == Error::BuildStatus::FAILURE) {
        delete program;
        exit(1);
    }

    // Execution
    std::cout << "Running program..." << std::endl;
    run(program);

    delete program;
}

void run(ProgramNode *program) {
    // Interpret the AST nodes
    auto *interpreter_visitor = new InterpreterVisitor();
    program->evaluate(interpreter_visitor, nullptr);
    delete interpreter_visitor;
}

void print_usage() {
    std::cout << "Usage: sscript <path>" << std::endl;
}
