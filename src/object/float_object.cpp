#include "object/int_object.h"
#include "object/float_object.h"
#include "object/bool_object.h"
#include "object/string_object.h"

std::shared_ptr<Object> FloatObject::add(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<FloatObject>(value + (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value + std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::sub(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<FloatObject>(value - (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value - std::static_pointer_cast<FloatObject>(other)->getValue());
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

std::shared_ptr<Object> FloatObject::mul(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<FloatObject>(value * (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value * std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::div(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<FloatObject>(value / (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>(value / std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::mod(std::shared_ptr<Object> other) {
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
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value == (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value == std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::notEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value != (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value != std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::lessThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value < (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value < std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::greaterThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value > (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value > std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::lessThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value <= (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value <= std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::greaterThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value >= (float)std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>(value >= std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> FloatObject::logicalAnd(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::logicalOr(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> FloatObject::logicalNot() {
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
