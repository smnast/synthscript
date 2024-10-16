#include "lexer.h"

std::vector<int> Lexer::line_prefix, Lexer::column_prefix;

std::vector<Token> Lexer::parse_tokens(std::string &file) {
    prepare_line_prefix(file);
    prepare_column_prefix(file);

    std::string token_regex = combine_regex();
    std::vector<Token> tokens;

    std::regex reg(token_regex);
    std::smatch match;

    auto start = file.cbegin(), end = file.cend();
    while (regex_search(start, end, match, reg)) {
        for (int i = 1; i < (int)match.size(); i++) {
            if (!(int)match[i].str().empty()) {
                int position = (int)(match[i].second - file.cbegin())-1;
                int line = get_line(position), column = get_column(position);
                Token cur_token(token_regexs[i - 1].first, match[i].str(), line, column);
                if (token_regexs[i-1].first == UNDEFINED) {
                    Lexer::lexer_error(match[i].str(), line, column);
                }
                else if (token_regexs[i-1].first != ESCAPED_NEW_LINE) {
                    tokens.push_back(cur_token);
                }

                start = match[i].second;
                break;
            }
        }
    }

    Token end_token(END_OF_FILE, "", get_line((int)file.size()-1), 1);
    tokens.push_back(end_token);
    return tokens;
}

std::string Lexer::combine_regex() {
    std::string token_regex;
    for (auto &token_expr : token_regexs) {
        token_regex += "(" + token_expr.second + ")";
        token_regex += "|";
    }
    token_regex.pop_back();
    return token_regex;
}

void Lexer::lexer_error(const std::string &token, int line, int col) {
    Error::error_at_pos("Undefined token: '" + token + "'", line, col);
}

void Lexer::prepare_line_prefix(std::string &file) {
    line_prefix.assign(file.size(), 0);
    int current_line = 1;
    for (int i = 0; i < (int)file.size(); i++) {
        line_prefix[i] = current_line;
        if (file[i] == '\n') current_line++;
    }
}

void Lexer::prepare_column_prefix(std::string &file) {
    column_prefix.assign(file.size(), 0);
    int last_line_start = 0;
    for (int i = 0; i < (int)file.size(); i++) {
        column_prefix[i] = last_line_start;
        if (file[i] == '\n') last_line_start = i+1;
    }
}

int Lexer::get_line(int position) {
    return line_prefix[position];
}

int Lexer::get_column(int position) {
    return position - column_prefix[position] + 1;
}
