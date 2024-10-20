#include "object/float_object.h"
#include "object/bool_object.h"
#include "object/int_object.h"
#include "object/string_object.h"

std::shared_ptr<Object> FloatObject::add(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<FloatObject>(
            value + (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            value + std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::subtract(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<FloatObject>(
            value - (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            value - std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::positive() {
    return std::make_shared<FloatObject>(value);
}

std::shared_ptr<Object> FloatObject::negative() {
    return std::make_shared<FloatObject>(-value);
}

std::shared_ptr<Object> FloatObject::multiply(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<FloatObject>(
            value * (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            value * std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::divide(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<FloatObject>(
            value / (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(
            value / std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::modulo(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::bitwise_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::bitwise_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::bitwise_xor(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::bitwise_not() {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value == (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value == std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::not_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value != (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value != std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::less_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value < (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value < std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::greater_than(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value > (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value > std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::less_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value <= (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value <= std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::greater_than_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        return std::make_shared<BoolObject>(
            value >= (float)std::static_pointer_cast<IntObject>(other)->get_value());
    } else if (other->get_type() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(
            value >= std::static_pointer_cast<FloatObject>(other)->get_value());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::logical_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::logical_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::logical_not() {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::cast(Type type) {
    if (type == TYPE_INT) {
        return std::make_shared<IntObject>((int)value);
    } else if (type == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value);
    } else if (type == TYPE_BOOL) {
        return std::make_shared<BoolObject>(value != 0.0f);
    } else if (type == TYPE_STRING) {
        return std::make_shared<StringObject>(std::to_string(value));
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::subscript(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::duplicate() {
    return std::make_shared<FloatObject>(value);
}

std::shared_ptr<Object> FloatObject::call(InterpreterVisitor *visitor, SymbolTable *table) {
    return nullptr;
}
