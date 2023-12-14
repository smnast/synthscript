#ifndef SYNTHSCRIPT_SYMBOL_H
#define SYNTHSCRIPT_SYMBOL_H

#include <memory>
#include <utility>

#include "Object/Object.h"

class Symbol {
public:
    Symbol() = default;
    Symbol(std::string name, std::shared_ptr<Object> value) : name(std::move(name)), value(std::move(value)) {}
    std::string getName() { return name; }
    std::shared_ptr<Object> getValue() { return value; }
    Type getType() { return value->getType(); }
private:
    std::string name;
    std::shared_ptr<Object> value;
};

#endif //SYNTHSCRIPT_SYMBOL_H
