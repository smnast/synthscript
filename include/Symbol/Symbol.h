#ifndef SYNTHSCRIPT_SYMBOL_H
#define SYNTHSCRIPT_SYMBOL_H

#include <memory>
#include <utility>

#include "Object/Object.h"

class Symbol {
public:
    Symbol() = default;
    explicit Symbol(std::string name) : name(std::move(name)), value(nullptr) {}
    Symbol(std::string name, std::shared_ptr<Object> value) : name(std::move(name)), value(std::move(value)) {}
    std::string getName() { return name; }
    void setValue(std::shared_ptr<Object> _value) { value = std::move(_value); }
    std::shared_ptr<Object> getValue() { return value; }
    Type getType() { return value == nullptr ? TYPE_UNDEF : value->getType(); }
private:
    std::string name;
    std::shared_ptr<Object> value;
};

#endif //SYNTHSCRIPT_SYMBOL_H
