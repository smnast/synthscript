#include "lexer.h"

std::vector<int> Lexer::linePrefix, Lexer::columnPrefix;

std::vector<Token> Lexer::parse_tokens(std::string &file) {
    prepare_line_prefix(file);
    prepare_column_prefix(file);

    std::string tokenRegex = combine_regex();
    std::vector<Token> tokens;

    std::regex reg(tokenRegex);
    std::smatch match;

    auto start = file.cbegin(), end = file.cend();
    while (regex_search(start, end, match, reg)) {
        for (int i = 1; i < (int)match.size(); i++) {
            if (!(int)match[i].str().empty()) {
                int position = (int)(match[i].second - file.cbegin())-1;
                int lineNumber = get_line(position), columnNumber = get_column(position);
                Token curToken(tokenRegexExprs[i - 1].first, match[i].str(), lineNumber, columnNumber);
                if (tokenRegexExprs[i-1].first == UNDEFINED) {
                    Lexer::lexer_error(match[i].str(), lineNumber, columnNumber);
                }
                else if (tokenRegexExprs[i-1].first != ESCAPED_NEW_LINE) {
                    tokens.push_back(curToken);
                }

                start = match[i].second;
                break;
            }
        }
    }

    Token endToken(END_OF_FILE, "", get_line((int)file.size()-1), 1);
    tokens.push_back(endToken);
    return tokens;
}

std::string Lexer::combine_regex() {
    std::string tokenRegex;
    for (auto &tokenExpr : tokenRegexExprs) {
        tokenRegex += "(" + tokenExpr.second + ")";
        tokenRegex += "|";
    }
    tokenRegex.pop_back();
    return tokenRegex;
}

void Lexer::lexer_error(const std::string &token, int line, int col) {
    Error::error_at_pos("Undefined token: '" + token + "'", line, col);
}

void Lexer::prepare_line_prefix(std::string &file) {
    linePrefix.assign(file.size(), 0);
    int currentLine = 1;
    for (int i = 0; i < (int)file.size(); i++) {
        linePrefix[i] = currentLine;
        if (file[i] == '\n') currentLine++;
    }
}

void Lexer::prepare_column_prefix(std::string &file) {
    columnPrefix.assign(file.size(), 0);
    int lastLineStart = 0;
    for (int i = 0; i < (int)file.size(); i++) {
        columnPrefix[i] = lastLineStart;
        if (file[i] == '\n') lastLineStart = i+1;
    }
}

int Lexer::get_line(int position) {
    return linePrefix[position];
}

int Lexer::get_column(int position) {
    return position - columnPrefix[position] + 1;
}
