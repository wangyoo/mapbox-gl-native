#include <mbgl/style/function/categorical_function.hpp>
#include <mbgl/style/types.hpp>
#include <mbgl/tile/geometry_tile_data.hpp>
#include <mbgl/util/color.hpp>

namespace mbgl {
namespace style {

optional<CategoricalFunctionDomainValue> domainValue(const optional<Value>& value) {
    using Result = optional<CategoricalFunctionDomainValue>;

    if (!value) {
        return {};
    }

    return value->match(
        [] (bool t) { return Result(t); },
        [] (uint64_t t) { return Result(int64_t(t)); },
        [] (int64_t t) { return Result(t); },
        [] (double t) { return Result(int64_t(t)); },
        [] (const std::string& t) { return Result(t); },
        [] (const auto&) { return Result(); }
    );
}

template <>
float CategoricalFunction<float>::evaluate(const GeometryTileFeature& feature) const {
    auto v = domainValue(feature.getValue(property));
    if (!v) {
        return 0.0f;
    }

    auto it = stops.find(*v);
    return it == stops.end() ? 0.0f : it->second;
}

template <>
Color CategoricalFunction<Color>::evaluate(const GeometryTileFeature& feature) const {
    auto v = domainValue(feature.getValue(property));
    if (!v) {
        return Color::black();
    }

    auto it = stops.find(*v);
    return it == stops.end() ? Color::black() : it->second;
}

} // namespace style
} // namespace mbgl
