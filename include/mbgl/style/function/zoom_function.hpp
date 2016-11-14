#pragma once

#include <cassert>
#include <utility>
#include <vector>

namespace mbgl {
namespace style {

template <typename T>
class ZoomFunction {
public:
    using DomainType = float;
    using RangeType = T;
    using Stop = std::pair<float, T>;
    using Stops = std::vector<Stop>;

    ZoomFunction(Stops stops_, float base_)
        : base(base_), stops(std::move(stops_)) {
        assert(stops.size() > 0);
    }

    float getBase() const { return base; }
    const std::vector<std::pair<float, T>>& getStops() const { return stops; }

    T evaluate(float z) const;

    friend bool operator==(const ZoomFunction& lhs, const ZoomFunction& rhs) {
        return lhs.base == rhs.base && lhs.stops == rhs.stops;
    }

    friend bool operator!=(const ZoomFunction& lhs, const ZoomFunction& rhs) {
        return !(lhs == rhs);
    }

private:
    float base = 1;
    std::vector<std::pair<float, T>> stops;
};

} // namespace style
} // namespace mbgl
