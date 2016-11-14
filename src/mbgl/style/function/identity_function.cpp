#include <mbgl/style/function/identity_function.hpp>
#include <mbgl/style/types.hpp>
#include <mbgl/tile/geometry_tile_data.hpp>
#include <mbgl/util/color.hpp>

namespace mbgl {
namespace style {

template <>
float IdentityFunction<float>::evaluate(const GeometryTileFeature& feature) const {
    optional<Value> v = feature.getValue(property);
    if (!v) {
        return 0.0f;
    }

    return numericValue<float>(*v)
        .value_or(0.0f);
}

template <>
Color IdentityFunction<Color>::evaluate(const GeometryTileFeature& feature) const {
    optional<Value> v = feature.getValue(property);
    if (!v || !v->is<std::string>()) {
        return Color::black();
    }

    return Color::parse(v->get<std::string>())
        .value_or(Color::black());
}

} // namespace style
} // namespace mbgl
