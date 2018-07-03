#pragma once
#include <memory>
#include <glm/glm.hpp>

#include "Renderer/ShaderVariable.h"

namespace Daybreak
{
    class IIndexBuffer;
    class IInputLayout;
    class IVertexBuffer;
    class IShader;
    class ITexture2d;

    // Render device context abstraction.
    class IRenderContext
    {
    public:
        // Constructor.
        IRenderContext() = default;

        // Copy constructor (deleted).
        IRenderContext(const IRenderContext&) = delete;

        // Base virtual destructor.
        virtual ~IRenderContext() = default;

        // Assignment operator (deleted).
        IRenderContext& operator =(const IRenderContext&) = delete;

    // Global methods.
    public:
        // Clear both color and depth buffer.
        //  TODO: Turn this into a clear method that takes flags for what to clear.
        virtual void clearColorAndDepthBuffers() = 0;

    // Resource binding.
    public:
        // Bind index buffer to pipeline.
        virtual void bindIndexBuffer(const std::shared_ptr<const IIndexBuffer>& indexBuffer) = 0;

        // Bind input layout to pipeline.
        virtual void bindInputLayout(const std::shared_ptr<const IInputLayout>& inputLayout) = 0;

        // Bind shader to pipeline.
        virtual void bindShader(const std::shared_ptr<const IShader>& shader) = 0;

        // Bind texture to pipeline.
        virtual void bindTexture(
            const std::shared_ptr<const ITexture2d>& texture,
            unsigned int slotIndex) = 0;

        // Bind vertex buffer to pipeline for rendering.
        virtual void bindVertexBuffer(const std::shared_ptr<const IVertexBuffer>& vertexBuffer) = 0;

    // Shader variables.
    public:
        // Set int shader parameter.
        virtual void setShaderInt(const ShaderVariable& v, int value) = 0;

        // Set float shader parameter.
        virtual void setShaderFloat(const ShaderVariable& v, float value) = 0;

        // Set bool shader parameter.
        virtual void setShaderBool(const ShaderVariable& v, bool value) = 0;

        // Set float2 shader parameter.
        virtual void setShaderVector2f(const ShaderVariable& v, float x, float y) = 0;

        // Set float2 shader parameter.
        virtual void setShaderVector2f(const ShaderVariable& v, const glm::vec2& vec) = 0;

        // Set float3 shader parameter.
        virtual void setShaderVector3f(const ShaderVariable& v, float x, float y, float z) = 0;

        // Set float3 shader parameter.
        virtual void setShaderVector3f(const ShaderVariable& v, const glm::vec3& vec) = 0;

        // Set float4 shader parameter.
        virtual void setShaderVector4f(const ShaderVariable& v, float x, float y, float z, float w) = 0;

        // Set float4 shader parameter.
        virtual void setShaderVector4f(const ShaderVariable& v, const glm::vec4& vec) = 0;

        // Set matrix4x4 shader parameter.
        virtual void setShaderMatrix3(const ShaderVariable& v, const glm::mat3& vec) = 0;

        // Set matrix4x4 shader parameter.
        virtual void setShaderMatrix4(const ShaderVariable& v, const glm::mat4& vec) = 0;

    // Contextual commands.
    public:
        virtual void drawTriangles(unsigned int offset, unsigned int count) = 0;

    // Properties.
    public:
        // Enable or disable depth testing.
        virtual void setDepthTestEnabled(bool isEnabled) = 0;

        // Check if depth testing is enabled.
        virtual bool isDepthTestEnabled() const = 0;

        // Enable or disable wireframe rendering.
        virtual void setWireframeEnabled(bool isEnabled) = 0;

        // Check if wireframe rendering is enabled.
        virtual bool isWireframeEnabled() const = 0;
    };
}