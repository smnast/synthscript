#include "Object/IntObject.h"
#include "Object/FloatObject.h"
#include "Object/BoolObject.h"
#include "Object/StringObject.h"

std::shared_ptr<Object> StringObject::add(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<StringObject>(value + std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::sub(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::positive() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::negative() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::mul(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        std::string result;
        for (int i = 0; i < std::static_pointer_cast<IntObject>(other)->getValue(); i++) {
            result += value;
        }
        return std::make_shared<StringObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::div(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::mod(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::bitwise_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::bitwise_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::bitwise_xor(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::bitwise_not() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::equal(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value == std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::notEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value != std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::lessThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value < std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::greaterThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value > std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::lessThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value <= std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::greaterThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value >= std::static_pointer_cast<StringObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::logicalAnd(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::logicalOr(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::logicalNot() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::cast(Type type) {
    if (type == TYPE_STRING) {
        return std::make_shared<StringObject>(value);
    } else if (type == TYPE_INT) {
        return std::make_shared<IntObject>(std::stoi(value));
    } else if (type == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(std::stof(value));
    } else if (type == TYPE_BOOL) {
        return std::make_shared<BoolObject>(value == "true");
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::subscript(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<StringObject>(std::string(1, value[std::static_pointer_cast<IntObject>(other)->getValue()]));
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::duplicate() {
    return std::make_shared<StringObject>(value);
}
