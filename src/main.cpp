#include "error_manager.h"
#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "tokens.h"
#include "visitor/print_visitor.h"
#include <iostream>

int build_and_run(const std::string &path);
void print_usage();

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *file_path = argv[1];
        return build_and_run(file_path);
    } else {
        print_usage();
        return 127;
    }
}

int build_and_run(const std::string &path) {
    std::cout << "Building program..." << std::endl;

    ErrorManager error_manager;

    // Read the file
    Reader reader(path, &error_manager);
    std::string code = reader.read_file();
    std::vector<std::string> file_lines = reader.get_lines();
    error_manager.set_file_lines(file_lines);

    // Lexical analysis
    Lexer lexer(code, &error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();

    // Syntax analysis
    Parser parser(tokens, &error_manager);
    ProgramNode *program = parser.parse_program();

    // Print the AST
    PrintVisitor print_visitor(program, &error_manager);
    print_visitor.print();

    // Semantic analysis
    SemanticAnalysisVisitor semantic_analysis_visitor(program, &error_manager);
    semantic_analysis_visitor.analyze();

    // Print build status
    std::string build_status =
        error_manager.get_status() == ErrorManager::BuildStatus::SUCCESS ? "SUCCESS" : "FAILURE";
    std::cout << "Build status:\t" << build_status << std::endl;
    std::cout << "Error count:\t" << error_manager.get_error_count() << std::endl;

    int exit_code = EXIT_SUCCESS;
    if (error_manager.get_status() == ErrorManager::BuildStatus::FAILURE) {
        delete program;
        exit_code = EXIT_FAILURE;
    } else {
        // Execution
        std::cout << "Running program..." << std::endl;

        try {
            // Interpret the AST nodes
            InterpreterVisitor interpreter_visitor(program, &error_manager);
            interpreter_visitor.interpret();
        } catch (const std::runtime_error &e) {
            exit_code = EXIT_FAILURE;
        }

        delete program;
    }

    return exit_code;
}

void print_usage() {
    std::cout << "Usage: sscript <path>" << std::endl;
}
