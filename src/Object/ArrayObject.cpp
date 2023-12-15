#include "Object/ArrayObject.h"
#include "Object/IntObject.h"
#include "Object/BoolObject.h"

std::shared_ptr<Object> ArrayObject::add(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> result;
        result.insert(result.end(), value.begin(), value.end());
        result.insert(result.end(), std::static_pointer_cast<ArrayObject>(other)->getValue()->begin(),
                      std::static_pointer_cast<ArrayObject>(other)->getValue()->end());
        return std::make_shared<ArrayObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::sub(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::positive() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::negative() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::mul(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        int times = std::static_pointer_cast<IntObject>(other)->getValue();
        std::vector<std::shared_ptr<Object>> result;
        for (int i = 0; i < times; i++) {
            auto cur = *std::static_pointer_cast<ArrayObject>(this->duplicate())->getValue();
            result.insert(result.end(), cur.begin(), cur.end());
        }
        return std::make_shared<ArrayObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::div(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::mod(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::bitwise_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::bitwise_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::bitwise_xor(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::bitwise_not() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::equal(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> otherValue = *std::static_pointer_cast<ArrayObject>(other)->getValue();
        bool match = true;
        if (value.size() != otherValue.size()) {
            for (int i = 0; i < value.size(); i++) {
                if (!otherValue[i]->equal(value[i])) {
                    match = false;
                    break;
                }
            }
        } else {
            match = false;
        }

        return std::make_shared<BoolObject>(match);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::notEqual(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> otherValue = *std::static_pointer_cast<ArrayObject>(other)->getValue();
        bool match = true;
        if (value.size() != otherValue.size()) {
            for (int i = 0; i < value.size(); i++) {
                if (!otherValue[i]->equal(value[i])) {
                    match = false;
                    break;
                }
            }
        } else {
            match = false;
        }

        return std::make_shared<BoolObject>(!match);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::lessThan(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::greaterThan(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::lessThanEqual(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::greaterThanEqual(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logicalAnd(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logicalOr(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logicalNot() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::cast(Type type) {
    if (type == TYPE_ARRAY) {
        return std::make_shared<ArrayObject>(value);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::subscript(std::shared_ptr<Object> other) {
    if (other->getType() == TYPE_INT) {
        int index = std::static_pointer_cast<IntObject>(other)->getValue();
        if (index < 0 || index >= value.size()) {
            return nullptr;
        }
        return value[index];
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::subscriptUpdate(const std::shared_ptr<Object>& index, const std::shared_ptr<Object> &val) {
    if (index->getType() == TYPE_INT) {
        int i = std::static_pointer_cast<IntObject>(index)->getValue();
        if (i < 0 || i >= value.size()) {
            return nullptr;
        }
        value[i] = val;
        return val;
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::duplicate() {
    std::vector<std::shared_ptr<Object>> result(value.size());
    for (int i = 0; i < value.size(); i++) {
        result[i] = value[i]->duplicate();
    }
    return std::make_shared<ArrayObject>(result);
}
