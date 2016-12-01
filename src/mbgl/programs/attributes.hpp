#pragma once

#include <mbgl/gl/attribute.hpp>
#include <mbgl/gl/normalization.hpp>

#include <cstdint>

namespace mbgl {
namespace attributes {

// Layout attributes

MBGL_DEFINE_ATTRIBUTE(int16_t, 2, a_pos);
MBGL_DEFINE_ATTRIBUTE(int16_t, 2, a_extrude);
MBGL_DEFINE_ATTRIBUTE(uint16_t, 2, a_texture_pos);

template <std::size_t N>
struct a_data : gl::Attribute<a_data<N>, uint8_t, N> {
    static constexpr auto name = "a_data";
};

template <std::size_t N>
struct a_offset : gl::Attribute<a_offset<N>, int16_t, N> {
    static constexpr auto name = "a_offset";
};

// Paint attributes

struct a_color : gl::Attribute<a_color, gl::Normalized<uint8_t>, 4> {
    static constexpr auto name = "a_color";

    static Value value(const Color& color) {
        return {{
            gl::Normalized<uint8_t>(color.r),
            gl::Normalized<uint8_t>(color.g),
            gl::Normalized<uint8_t>(color.b),
            gl::Normalized<uint8_t>(color.a)
        }};
    }
};

struct a_outline_color : gl::Attribute<a_outline_color, gl::Normalized<uint8_t>, 4> {
    static constexpr auto name = "a_outline_color";

    static Value value(const Color& color) {
        return {{
            gl::Normalized<uint8_t>(color.r),
            gl::Normalized<uint8_t>(color.g),
            gl::Normalized<uint8_t>(color.b),
            gl::Normalized<uint8_t>(color.a)
        }};
    }
};

struct a_opacity : gl::Attribute<a_opacity, gl::Normalized<uint8_t>, 1> {
    static constexpr auto name = "a_opacity";

    static Value value(float opacity) {
        return {{ gl::Normalized<uint8_t>(opacity) }};
    }
};

struct a_blur : gl::Attribute<a_blur, float, 1> {
    static constexpr auto name = "a_blur";

    static Value value(float blur) {
        return {{ blur }};
    }
};

struct a_radius : gl::Attribute<a_radius, float, 1> {
    static constexpr auto name = "a_radius";

    static Value value(float radius) {
        return {{ radius }};
    }
};

struct a_width : gl::Attribute<a_width, float, 1> {
    static constexpr auto name = "a_width";

    static Value value(float width) {
        return {{ width }};
    }
};

struct a_gap_width : gl::Attribute<a_gap_width, float, 1> {
    static constexpr auto name = "a_gapwidth";

    static Value value(float width) {
        return {{ width }};
    }
};

template <>
struct a_offset<1> : gl::Attribute<a_offset<1>, int16_t, 1> {
    static constexpr auto name = "a_offset";

    static Value value(int16_t offset) {
        return {{ offset }};
    }
};

} // namespace attributes
} // namespace mbgl
