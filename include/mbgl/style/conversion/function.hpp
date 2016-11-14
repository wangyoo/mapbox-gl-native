#pragma once

#include <mbgl/style/function/zoom_function.hpp>
#include <mbgl/style/function/property_function.hpp>
#include <mbgl/style/function/identity_function.hpp>
#include <mbgl/style/function/categorical_function.hpp>
#include <mbgl/style/conversion.hpp>
#include <mbgl/style/conversion/constant.hpp>

namespace mbgl {
namespace style {
namespace conversion {

template <class FunctionType, class V>
Result<typename FunctionType::Stops> convertStops(const V& value) {
    auto stopsValue = objectMember(value, "stops");
    if (!stopsValue) {
        return Error { "function value must specify stops" };
    }

    if (!isArray(*stopsValue)) {
        return Error { "function stops must be an array" };
    }

    if (arrayLength(*stopsValue) == 0) {
        return Error { "function must have at least one stop" };
    }

    using Stops = typename FunctionType::Stops;
    using D = typename FunctionType::DomainType;
    using R = typename FunctionType::RangeType;

    Stops stops;
    for (std::size_t i = 0; i < arrayLength(*stopsValue); ++i) {
        const auto& stopValue = arrayMember(*stopsValue, i);

        if (!isArray(stopValue)) {
            return Error { "function stop must be an array" };
        }

        if (arrayLength(stopValue) != 2) {
            return Error { "function stop must have two elements" };
        }

        Result<D> d = convert<D>(arrayMember(stopValue, 0));
        if (!d) {
            return d.error();
        }

        Result<R> r = convert<R>(arrayMember(stopValue, 1));
        if (!r) {
            return r.error();
        }

        stops.insert(stops.end(), std::make_pair(*d, *r));
    }

    return stops;
}

template <class T, class V>
Result<float> convertBase(const V& value) {
    auto baseValue = objectMember(value, "base");
    if (!baseValue) {
        return 1.0f;
    }

    optional<float> base = toNumber(*baseValue);
    if (!base) {
        return Error { "function base must be a number"};
    }

    return *base;
}

template <class T>
struct Converter<ZoomFunction<T>> {
    template <class V>
    Result<ZoomFunction<T>> operator()(const V& value) const {
        if (!isObject(value)) {
            return Error { "function must be an object" };
        }

        auto stops = convertStops<ZoomFunction<T>>(value);
        if (!stops) {
            return stops.error();
        }

        auto base = convertBase<T>(value);
        if (!base) {
            return base.error();
        }

        return ZoomFunction<T>(*stops, *base);
    }
};

template <class T>
struct Converter<PropertyFunction<T>> {
    template <class V>
    Result<PropertyFunction<T>> operator()(const V& value) const {
        if (!isObject(value)) {
            return Error { "function must be an object" };
        }

        auto stops = convertStops<PropertyFunction<T>>(value);
        if (!stops) {
            return stops.error();
        }

        auto base = convertBase<T>(value);
        if (!base) {
            return base.error();
        }

        auto propertyValue = objectMember(value, "property");
        if (!propertyValue) {
            return Error { "function must specify property" };
        }

        auto propertyString = toString(*propertyValue);
        if (!propertyString) {
            return Error { "function property must be a string" };
        }

        return PropertyFunction<T>(*propertyString, *stops, *base);
    }
};

template <class T>
struct Converter<IdentityFunction<T>> {
    template <class V>
    Result<IdentityFunction<T>> operator()(const V& value) const {
        if (!isObject(value)) {
            return Error { "function must be an object" };
        }

        auto propertyValue = objectMember(value, "property");
        if (!propertyValue) {
            return Error { "function must specify property" };
        }

        auto propertyString = toString(*propertyValue);
        if (!propertyString) {
            return Error { "function property must be a string" };
        }

        return IdentityFunction<T>(*propertyString);
    }
};

template <>
struct Converter<CategoricalFunctionDomainValue> {
    template <class V>
    Result<CategoricalFunctionDomainValue> operator()(const V& value) const {
        auto b = toBool(value);
        if (b) {
            return *b;
        }

        auto n = toNumber(value);
        if (n) {
            return int64_t(*n);
        }

        auto s = toString(value);
        if (s) {
            return *s;
        }

        return Error { "stop domain value must be a number, string, or boolean" };
    }
};

template <class T>
struct Converter<CategoricalFunction<T>> {
    template <class V>
    Result<CategoricalFunction<T>> operator()(const V& value) const {
        if (!isObject(value)) {
            return Error { "function must be an object" };
        }

        auto stops = convertStops<CategoricalFunction<T>>(value);
        if (!stops) {
            return stops.error();
        }

        auto propertyValue = objectMember(value, "property");
        if (!propertyValue) {
            return Error { "function must specify property" };
        }

        auto propertyString = toString(*propertyValue);
        if (!propertyString) {
            return Error { "function property must be a string" };
        }

        return CategoricalFunction<T>(*propertyString, *stops);
    }
};

} // namespace conversion
} // namespace style
} // namespace mbgl
