#include "Lexer.h"

std::vector<int> Lexer::linePrefix, Lexer::columnPrefix;

std::vector<Token> Lexer::parseTokens(std::string &file) {
    prepareLinePrefix(file);
    prepareColumnPrefix(file);

    std::string tokenRegex = combineRegex();
    std::vector<Token> tokens;

    std::regex reg(tokenRegex);
    std::smatch match;

    auto start = file.cbegin(), end = file.cend();
    while (regex_search(start, end, match, reg)) {
        for (int i = 1; i < (int)match.size(); i++) {
            if (!(int)match[i].str().empty()) {
                int position = (int)(match[i].second - file.cbegin())-1;
                int lineNumber = getLineNumber(position), columnNumber = getColumnNumber(position);
                Token curToken(tokenRegexExprs[i - 1].first, match[i].str(), lineNumber, columnNumber);
                if (tokenRegexExprs[i-1].first == UNDEFINED) {
                    Lexer::lexerError(match[i].str(), lineNumber, columnNumber);
                }
                else {
                    tokens.push_back(curToken);
                }

                start = match[i].second;
                break;
            }
        }
    }

    Token endToken(END_OF_FILE, "", getLineNumber((int)file.size()-1), 1);
    tokens.push_back(endToken);
    return tokens;
}

std::string Lexer::combineRegex() {
    std::string tokenRegex;
    for (auto &tokenExpr : tokenRegexExprs) {
        tokenRegex += "(" + tokenExpr.second + ")";
        tokenRegex += "|";
    }
    tokenRegex.pop_back();
    return tokenRegex;
}

void Lexer::lexerError(const std::string &token, int line, int col) {
    Error::posError("Undefined token: '" + token + "'", line, col);
}

void Lexer::prepareLinePrefix(std::string &file) {
    linePrefix.assign(file.size(), 0);
    int currentLine = 1;
    for (int i = 0; i < (int)file.size(); i++) {
        linePrefix[i] = currentLine;
        if (file[i] == '\n') currentLine++;
    }
}

void Lexer::prepareColumnPrefix(std::string &file) {
    columnPrefix.assign(file.size(), 0);
    int lastLineStart = 0;
    for (int i = 0; i < (int)file.size(); i++) {
        columnPrefix[i] = lastLineStart;
        if (file[i] == '\n') lastLineStart = i+1;
    }
}

int Lexer::getLineNumber(int position) {
    return linePrefix[position];
}

int Lexer::getColumnNumber(int position) {
    return position - columnPrefix[position] + 1;
}
