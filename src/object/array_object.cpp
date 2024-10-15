#include "object/array_object.h"
#include "object/int_object.h"
#include "object/bool_object.h"

std::shared_ptr<Object> ArrayObject::add(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> result;
        result.insert(result.end(), value.begin(), value.end());
        result.insert(result.end(), std::static_pointer_cast<ArrayObject>(other)->get_value()->begin(),
                      std::static_pointer_cast<ArrayObject>(other)->get_value()->end());
        return std::make_shared<ArrayObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::subtract(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::positive() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::negative() {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::multiply(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_INT) {
        int times = std::static_pointer_cast<IntObject>(other)->get_value();
        std::vector<std::shared_ptr<Object>> result;
        for (int i = 0; i < times; i++) {
            auto cur = *std::static_pointer_cast<ArrayObject>(this->duplicate())->get_value();
            result.insert(result.end(), cur.begin(), cur.end());
        }
        return std::make_shared<ArrayObject>(result);
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::divide(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::modulo(std::shared_ptr<Object> other) {
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
    if (other->get_type() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> otherValue = *std::static_pointer_cast<ArrayObject>(other)->get_value();
        bool match = true;
        if (value.size() == otherValue.size()) {
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

std::shared_ptr<Object> ArrayObject::not_equal(std::shared_ptr<Object> other) {
    if (other->get_type() == TYPE_ARRAY) {
        std::vector<std::shared_ptr<Object>> otherValue = *std::static_pointer_cast<ArrayObject>(other)->get_value();
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

std::shared_ptr<Object> ArrayObject::less_than(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::greater_than(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::less_than_equal(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::greater_than_equal(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logical_and(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logical_or(std::shared_ptr<Object> other) {
    return nullptr;
}

std::shared_ptr<Object> ArrayObject::logical_not() {
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
    if (other->get_type() == TYPE_INT) {
        int index = std::static_pointer_cast<IntObject>(other)->get_value();
        if (index < 0 || index >= value.size()) {
            return nullptr;
        }
        return value[index];
    } else {
        return nullptr;
    }
}

std::shared_ptr<Object> ArrayObject::subscriptUpdate(const std::shared_ptr<Object>& index, const std::shared_ptr<Object> &val) {
    if (index->get_type() == TYPE_INT) {
        int i = std::static_pointer_cast<IntObject>(index)->get_value();
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
