#include "symbol/symbol.h"
#include <memory>
#include <utility>

Symbol::Symbol(std::string name) : name(std::move(name)), value(nullptr) {}

Symbol::Symbol(std::string name, std::shared_ptr<Object> value)
    : name(std::move(name)), value(std::move(value)) {}

std::string Symbol::get_name() const {
    return name;
}

void Symbol::set_value(std::shared_ptr<Object> value) {
    this->value = std::move(value);
}

std::shared_ptr<Object> Symbol::get_value() const {
    return value;
}

Type Symbol::get_type() const {
    return value->get_type();
}
