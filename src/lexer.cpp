#include "lexer.h"
#include "error.h"
#include <regex>

std::vector<int> Lexer::line_prefix, Lexer::line_offset_prefix;

std::vector<Token> Lexer::parse_tokens(std::string &code) {
    // Preparation for lexical analysis
    prepare_prefixes(code);
    prepare_prefixes(code);
    std::string token_regex = combine_regex();

    // Lexical analysis
    std::vector<Token> tokens = get_tokens(code, token_regex);

    return tokens;
}

std::string Lexer::combine_regex() {
    // Combine all the regexes into one with the format:
    // (regex_1)|(regex_2)|...|(regex_N)
    std::string token_regex;
    for (auto &token_expr : token_regexs) {
        token_regex += "(" + token_expr.second + ")";
        token_regex += "|";
    }

    // Remove extra separator ('|')
    token_regex.pop_back();

    return token_regex;
}

void Lexer::lexer_error(const std::string &token, int line, int col) {
    Error::error_at_pos("Undefined token: '" + token + "'", line, col);
}

void Lexer::prepare_prefixes(std::string &code) {
    // Add one for EOF token
    line_prefix.assign(code.size() + 1, 0);
    line_offset_prefix.assign(code.size() + 1, 0);

    // Prepare the line and line offset prefixes
    int last_line_start = 0;
    int current_line = 1;
    for (int i = 0; i < (int)code.size(); i++) {
        line_offset_prefix[i] = last_line_start;
        line_prefix[i] = current_line;

        // New line
        if (code[i] == '\n') {
            last_line_start = i + 1;
            current_line++;
        }
    }
}

std::vector<Token> Lexer::get_tokens(std::string &code, std::string &token_regex) {
    std::vector<Token> tokens;

    // Prepare regex
    std::regex reg(token_regex);
    std::smatch match;

    // Tokenize the code
    auto start = code.cbegin(), end = code.cend();
    while (regex_search(start, end, match, reg)) {
        // First match is the entire regex, the rest are the individual groups
        for (int i = 1; i < (int)match.size(); i++) {
            int token_idx = i - 1;
            bool is_match_empty = match[i].str().empty();
            if (!is_match_empty) {
                // Parse the token
                int position = (int)(match[i].second - code.cbegin()) - 1;
                int line = get_line(position), column = get_column(position);
                Token cur_token(token_regexs[token_idx].first, match[i].str(), line, column);

                // Check for undefined tokens
                if (token_regexs[token_idx].first == UNDEFINED) {
                    Lexer::lexer_error(match[i].str(), line, column);
                    // Don't include new line escapes in the token list
                } else if (token_regexs[token_idx].first != ESCAPED_NEW_LINE) {
                    tokens.push_back(cur_token);
                }

                // Move the start position to the end of the current match
                start = match[i].second;
                break;
            }
        }
    }

    // Add the end of file token
    Token end_token(END_OF_FILE, "", get_line(code.size()), 1);
    tokens.push_back(end_token);

    return tokens;
}

int Lexer::get_line(size_t position) {
    return line_prefix[position];
}

int Lexer::get_column(size_t position) {
    return position - line_offset_prefix[position] + 1;
}
