#include "shortcuts.h"
#include "lexer.h"
#include "parser.h"
#include "reader.h"
#include "temp_file.h"

ProgramNode *parse_program(ErrorManager *error_manager, std::string file_path, std::string code) {
    TempFile temp_file(file_path, code);
    Reader reader(file_path, error_manager);
    Lexer lexer(reader.read_file(), error_manager);
    Parser parser(lexer.parse_tokens(), error_manager);
    return parser.parse_program();
}

std::vector<Token>
lex_tokens(ErrorManager *error_manager, std::string file_path, std::string code) {
    TempFile temp_file(file_path, code);
    Reader reader(file_path, error_manager);
    Lexer lexer(reader.read_file(), error_manager);
    std::vector<Token> tokens = lexer.parse_tokens();
    return tokens;
}
