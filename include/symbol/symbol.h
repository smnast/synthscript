#ifndef SYNTHSCRIPT_SYMBOL_H
#define SYNTHSCRIPT_SYMBOL_H

#include "object/object.h"
#include <memory>
#include <utility>

class Symbol {
public:
    Symbol() = default;
    explicit Symbol(std::string name) : name(std::move(name)), value(nullptr) {}
    Symbol(std::string name, std::shared_ptr<Object> value)
        : name(std::move(name)), value(std::move(value)) {}

    std::string get_name() const { return name; }
    void set_value(std::shared_ptr<Object> value) { this->value = std::move(value); }
    std::shared_ptr<Object> get_value() const { return value; }
    Type get_type() const { return value == nullptr ? TYPE_UNDEF : value->get_type(); }

private:
    std::string name;
    std::shared_ptr<Object> value;
};

#endif // SYNTHSCRIPT_SYMBOL_H
