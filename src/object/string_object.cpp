#include "object/string_object.h"
#include "object/bool_object.h"
#include "object/float_object.h"
#include "object/int_object.h"

std::shared_ptr<Object> StringObject::add(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<StringObject>(value + std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::subtract(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::positive() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::negative() {
    return nullptr;
}

std::shared_ptr<Object> StringObject::multiply(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        std::string result;
        for (int i = 0; i < std::static_pointer_cast<IntObject>(other)->get_value(); i++) {
            result += value;
        }
        return std::make_shared<StringObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::divide(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::modulo(std::shared_ptr<Object> other) {
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
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value == std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::not_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value != std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::less_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value < std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::greater_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value > std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::less_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value <= std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::greater_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_STRING) {
        return std::make_shared<BoolObject>(value >= std::static_pointer_cast<StringObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::logical_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::logical_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> StringObject::logical_not() {
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
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<StringObject>(std::string(1, value[std::static_pointer_cast<IntObject>(other)->get_value()]));
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> StringObject::duplicate() {
    return std::make_shared<StringObject>(value);
}
