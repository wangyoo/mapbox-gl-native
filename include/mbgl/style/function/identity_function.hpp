#pragma once

#include <string>

namespace mbgl {

class GeometryTileFeature;

namespace style {

template <typename T>
class IdentityFunction {
public:
    IdentityFunction(std::string property_)
        : property(std::move(property_)) {}

    const std::string& getProperty() const { return property; }

    T evaluate(const GeometryTileFeature&) const;

    friend bool operator==(const IdentityFunction& lhs, const IdentityFunction& rhs) {
        return lhs.property == rhs.property;
    }

    friend bool operator!=(const IdentityFunction& lhs, const IdentityFunction& rhs) {
        return !(lhs == rhs);
    }

private:
    std::string property;
};

} // namespace style
} // namespace mbgl
