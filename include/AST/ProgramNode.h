#ifndef CLIKE_PROGRAMNODE_H
#define CLIKE_PROGRAMNODE_H

#include <utility>
#include <vector>

#include "ASTNode.h"

class ProgramNode : public ASTNode {
public:
    explicit ProgramNode(std::vector<ASTNode*> statements, int line, int col) : ASTNode(line, col), statements(std::move(statements)) {}
    ~ProgramNode() override = default;
    std::vector<ASTNode*> *getStatements() { return &statements; }
    DECLARE_VISITOR_FUNCTIONS
private:
    std::vector<ASTNode*> statements;
};

#endif //CLIKE_PROGRAMNODE_H
