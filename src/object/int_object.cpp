#include "object/int_object.h"
#include "object/float_object.h"
#include "object/bool_object.h"
#include "object/string_object.h"

std::shared_ptr<Object> IntObject::add(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value + std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float)value + std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::sub(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value - std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float)value - std::static_pointer_cast<FloatObject>(other)->getValue());
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

std::shared_ptr<Object> IntObject::mul(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value * std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float)value * std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::div(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value / std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float)value / std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::mod(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value % std::static_pointer_cast<IntObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_and(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value & std::static_pointer_cast<IntObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_or(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value | std::static_pointer_cast<IntObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_xor(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<IntObject>(value ^ std::static_pointer_cast<IntObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::bitwise_not() {
    return std::make_shared<IntObject>(~value);
}

std::shared_ptr<Object> IntObject::equal(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value == std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value == std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::notEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value != std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value != std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::lessThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value < std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value < std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::greaterThan(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value > std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value > std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::lessThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value <= std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value <= std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::greaterThanEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        return std::make_shared<BoolObject>(value >= std::static_pointer_cast<IntObject>(other)->getValue());
    } else if (other->getType() == TYPE_FLOAT) {
        return std::make_shared<BoolObject>((float)value >= std::static_pointer_cast<FloatObject>(other)->getValue());
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> IntObject::logicalAnd(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> IntObject::logicalOr(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> IntObject::logicalNot() {
    return nullptr;
}

std::shared_ptr<Object> IntObject::cast(Type type) {
    if (type == TYPE_INT) {
        return std::make_shared<IntObject>(value);
    } else if (type == TYPE_FLOAT) {
        return std::make_shared<FloatObject>((float) value);
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
