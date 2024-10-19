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
    // TODO: handle case where the file path is provided but the file does not exist / other error
    // with the file
    bool valid_file_path = argc == 2 && Reader::file_exists(argv[1]);

    if (valid_file_path) {
        build_and_run(argv[1]);
    } else {
        print_usage();
    }
}

void build_and_run(const std::string &path) {
    // Lexical analysis
    std::string code = Reader::read_file(path);
    std::vector<Token> tokens = Lexer::parse_tokens(code);

    // Syntax analysis
    ProgramNode *program = Parser::parse_program(tokens);

    // Print the AST
    auto *print_visitor = new PrintVisitor();
    program->accept(print_visitor, 0);
    delete print_visitor;

    // Semantic analysis
    auto *semantic_analysis_visitor = new SemanticAnalysisVisitor();
    program->analyze(semantic_analysis_visitor, nullptr);
    delete semantic_analysis_visitor;

    Error::print_build_status();
    if (Error::should_quit()) {
        delete program;
        exit(1);
    }

    // Execution
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
