#ifndef CLIKE_PROGRAMNODE_H
#define CLIKE_PROGRAMNODE_H

#include <utility>
#include <vector>

#include "ASTNode.h"

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::vector<ASTNode*> statements, int lineNumber) : ASTNode(lineNumber), statements(std::move(statements)) {}
    ~ProgramNode() override = default;
    std::vector<ASTNode*> *getStatements() { return &statements; }
private:
    std::vector<ASTNode*> statements;
};

#endif //CLIKE_PROGRAMNODE_H
