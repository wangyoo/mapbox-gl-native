#pragma once

#include <mbgl/style/property_value.hpp>
#include <mbgl/style/property_evaluation_parameters.hpp>
#include <mbgl/style/possibly_evaluated_property_value.hpp>

namespace mbgl {
namespace style {

template <typename T>
class DataDrivenPropertyEvaluator {
public:
    using ResultType = PossiblyEvaluatedPropertyValue<T>;

    DataDrivenPropertyEvaluator(const PropertyEvaluationParameters& parameters_, T defaultValue_)
        : parameters(parameters_),
          defaultValue(std::move(defaultValue_)) {}

    ResultType operator()(const Undefined&) const { return defaultValue; }
    ResultType operator()(const T& constant) const { return constant; }
    ResultType operator()(const ZoomFunction<T>& f) const { return f.evaluate(parameters.z); }
    ResultType operator()(const PropertyFunction<T>& f) const { return f; }
    ResultType operator()(const IdentityFunction<T>& f) const { return f; }
    ResultType operator()(const CategoricalFunction<T>& f) const { return f; }

private:
    const PropertyEvaluationParameters& parameters;
    T defaultValue;
};

} // namespace style
} // namespace mbgl
