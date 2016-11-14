#pragma once

#include <mbgl/util/variant.hpp>
#include <mbgl/style/undefined.hpp>
#include <mbgl/style/function/zoom_function.hpp>
#include <mbgl/style/function/property_function.hpp>
#include <mbgl/style/function/identity_function.hpp>
#include <mbgl/style/function/categorical_function.hpp>

namespace mbgl {
namespace style {

template <class T>
class DataDrivenPropertyValue {
private:
    using Value = variant<
        Undefined,
        T,
        ZoomFunction<T>,
        PropertyFunction<T>,
        IdentityFunction<T>,
        CategoricalFunction<T>>;

    Value value;

    friend bool operator==(const DataDrivenPropertyValue& lhs, const DataDrivenPropertyValue& rhs) {
        return lhs.value == rhs.value;
    }

    friend bool operator!=(const DataDrivenPropertyValue& lhs, const DataDrivenPropertyValue& rhs) {
        return !(lhs == rhs);
    }

public:
    DataDrivenPropertyValue()                                : value()         {}
    DataDrivenPropertyValue(                    T  constant) : value(constant) {}
    DataDrivenPropertyValue(       ZoomFunction<T> function) : value(function) {}
    DataDrivenPropertyValue(   PropertyFunction<T> function) : value(function) {}
    DataDrivenPropertyValue(   IdentityFunction<T> function) : value(function) {}
    DataDrivenPropertyValue(CategoricalFunction<T> function) : value(function) {}

    bool isUndefined() const { return value.template is<Undefined>(); }

    explicit operator bool() const { return !isUndefined(); };

    template <typename Evaluator>
    auto evaluate(const Evaluator& evaluator) const {
        return Value::visit(value, evaluator);
    }
};

} // namespace style
} // namespace mbgl
