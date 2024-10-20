#include "object/int_object.h"
#include "object/bool_object.h"
#include "object/float_object.h"
#include "object/string_object.h"

std::shared_ptr<Object> IntObject::add(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value +
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            (float)value + std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::subtract(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value -
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            (float)value - std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::positive() {
    return std::make_shared<IntObject>(value);
}

std::shared_ptr<Object> IntObject::negative() {
    return std::make_shared<IntObject>(-value);
}

std::shared_ptr<Object> IntObject::multiply(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value *
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            (float)value * std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::divide(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value /
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            (float)value / std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::modulo(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value %
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_and(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value &
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_or(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value |
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_xor(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<IntObject>(value ^
                                           std::static_pointer_cast<IntObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_not() {
    return std::make_shared<IntObject>(~value);
}

std::shared_ptr<Object> IntObject::equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value == std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value == std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::not_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value != std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value != std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::less_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value < std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value < std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::greater_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value > std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value > std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::less_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value <= std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value <= std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::greater_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value >= std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            (float)value >= std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::logical_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> IntObject::logical_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> IntObject::logical_not() {
    return nullptr;
}

std::shared_ptr<Object> IntObject::cast(Type type) {
    if (type == TYPE_INT) {
        return std::make_shared<IntObject>(value);
    } else if (type == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float)value);
    } else if (type == TYPE_BOOL) {
        return std::make_shared<BoolObject>(value != 0);
    } else if (type == TYPE_STRING) {
        return std::make_shared<StringObject>(std::to_string(value));
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::subscript(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> IntObject::duplicate() {
    return std::make_shared<IntObject>(value);
}

std::shared_ptr<Object> IntObject::call(InterpreterVisitor *visitor, SymbolTable *table) {
    return nullptr;
}
