#pragma once

#include <mbgl/gl/program.hpp>
#include <mbgl/programs/paint_attribute_data.hpp>
#include <mbgl/programs/program_parameters.hpp>
#include <mbgl/programs/attributes.hpp>
#include <mbgl/style/paint_property.hpp>

#include <sstream>
#include <cassert>

namespace mbgl {

template <class Shaders,
          class Primitive,
          class LayoutAttrs,
          class Uniforms,
          class PaintProperties>
class Program {
public:
    using LayoutAttributes = LayoutAttrs;
    using LayoutVertex = typename LayoutAttributes::Vertex;

    using PaintAttributeData = PaintAttributeData<typename PaintProperties::Properties,
                                                  typename PaintProperties::DataDrivenProperties>;
    using PaintAttributeBindings = typename PaintAttributeData::Attributes::Bindings;
    using PaintAttributes = typename PaintAttributeData::Attributes;

    using Attributes = gl::ConcatenateAttributes<LayoutAttributes, PaintAttributes>;
    using ProgramType = gl::Program<Primitive, Attributes, Uniforms>;
    using UniformValues = typename ProgramType::UniformValues;

    ProgramType program;

    Program(gl::Context& context, const ProgramParameters& programParameters)
        : program(context, vertexSource(programParameters), fragmentSource(programParameters))
        {}
    
    static std::string pixelRatioDefine(const ProgramParameters& parameters) {
        std::ostringstream pixelRatioSS;
        pixelRatioSS.imbue(std::locale("C"));
        pixelRatioSS.setf(std::ios_base::showpoint);
        pixelRatioSS << parameters.pixelRatio;
        return std::string("#define DEVICE_PIXEL_RATIO ") + pixelRatioSS.str() + "\n";
    }

    static std::string fragmentSource(const ProgramParameters& parameters) {
        std::string source = pixelRatioDefine(parameters) + Shaders::fragmentSource;
        if (parameters.overdraw) {
            assert(source.find("#ifdef OVERDRAW_INSPECTOR") != std::string::npos);
            source.replace(source.find_first_of('\n'), 1, "\n#define OVERDRAW_INSPECTOR\n");
        }
        return source;
    }

    static std::string vertexSource(const ProgramParameters& parameters) {
        return pixelRatioDefine(parameters) + Shaders::vertexSource;
    }

    template <class DrawMode>
    void draw(gl::Context& context,
              DrawMode drawMode,
              gl::DepthMode depthMode,
              gl::StencilMode stencilMode,
              gl::ColorMode colorMode,
              UniformValues&& uniformValues,
              const gl::VertexBuffer<LayoutVertex>& layoutVertexBuffer,
              const gl::IndexBuffer<DrawMode>& indexBuffer,
              const gl::SegmentVector<Attributes>& segments,
              const PaintAttributeBindings& paintAttributeBindings) {
        program.draw(
            context,
            std::move(drawMode),
            std::move(depthMode),
            std::move(stencilMode),
            std::move(colorMode),
            std::move(uniformValues),
            LayoutAttributes::allVariableBindings(layoutVertexBuffer)
                .concat(paintAttributeBindings),
            indexBuffer,
            segments
        );
    }
};

} // namespace mbgl
