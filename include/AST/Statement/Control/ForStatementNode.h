#ifndef CLIKE_FORSTATEMENTNODE_H
#define CLIKE_FORSTATEMENTNODE_H

#include <utility>

#include "AST/ASTNode.h"

class ForStatementNode : public ASTNode {
public:
    ForStatementNode(std::string identifier, ASTNode *start, ASTNode *end, ASTNode *body, int line, int col) : ASTNode(line, col), identifier(std::move(identifier)), start(start), end(end), body(body) {}
    ~ForStatementNode() override {
        delete start;
        delete end;
        delete body;
    }
    std::string getIdentifier() { return identifier; }
    ASTNode *getStart() { return start; }
    ASTNode *getEnd() { return end; }
    ASTNode *getBody() { return body; }
private:
    std::string identifier;
    ASTNode *start, *end, *body;
};

#endif //CLIKE_FORSTATEMENTNODE_H
