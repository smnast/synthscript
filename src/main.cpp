#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "tokens.h"
#include "visitor/print_visitor.h"

void build_and_run(const std::string &path);
void run(ProgramNode *program);
void print_usage();

int main(int argc, char *argv[]) {
    if (argc == 2) {
        char *file_path = argv[1];
        if (!Reader::file_exists(file_path)) {
            printf("File not found: %s\n", file_path);
        } else {
            build_and_run(file_path);
        }
    } else {
        print_usage();
    }
}

void build_and_run(const std::string &path) {
    printf("Building program...\n");

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
    std::string build_status = Error::get_status() == Error::BuildStatus::SUCCESS ? "SUCCESS" : "FAILURE";
    printf("Build status:\t%s\n", build_status.c_str());
    printf("Error count:\t%d\n", Error::get_error_count());

    if (Error::get_status() == Error::BuildStatus::FAILURE) {
        delete program;
        exit(1);
    }

    // Execution
    printf("Running program...\n");
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
    std::printf("Usage: sscript <path>\n");
}
