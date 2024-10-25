#ifndef SYNTHSCRIPT_SHORTCUTS_H
#define SYNTHSCRIPT_SHORTCUTS_H

#include "AST/program_node.h"
#include "error_manager.h"
#include "tokens.h"
#include <string>
#include <vector>

/**
 * @brief Parse the program and return the root node of the AST.
 * @param error_manager The error manager to use for error handling.
 * @param file_path The path to the file to parse.
 * @param code The code to parse.
 * @return A pointer to the root node of the AST.
 *
 * @note
 * The caller is responsible for deleting the returned node.
 */
ProgramNode *parse_program(ErrorManager *error_manager, std::string file_path, std::string code);

/**
 * @brief Lex the tokens of the code.
 * @param error_manager The error manager to use for error handling.
 * @param file_path The path of the file.
 * @param code The code to be lexed.
 * @return std::vector<Token> The lexed tokens.
 */
std::vector<Token> lex_tokens(ErrorManager *error_manager, std::string file_path, std::string code);

#endif // SYNTHSCRIPT_SHORTCUTS_H
