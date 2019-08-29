#pragma once
#include "Renderer/RenderContext.h"
#include "Renderer/IndexBuffer.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL implementation of render context.
    class OglRenderContext final : public IRenderContext
    {
    public:
        // Constructor.
        OglRenderContext();

        // Destructor.
        ~OglRenderContext();

    // Global methods.
    public:
        // Clear both color and depth buffer.
        //  TODO: Turn this into a clear method that takes flags for what to clear.
        virtual void clearColorAndDepthBuffers() override;

    // Resource binding.
    public:
        // Bind index buffer to pipeline.
        virtual void bindIndexBuffer(const std::shared_ptr<const Daybreak::IndexBuffer>& indexBuffer) override;

        // Bind input layout to pipeline.
        virtual void bindInputLayout(const std::shared_ptr<const Daybreak::InputLayout>& inputLayout) override;

        // Bind shader to pipeline.
        virtual void bindShader(const std::shared_ptr<const Daybreak::IShader>& shader) override;

        // Bind texture to pipeline.
        virtual void bindTexture(
            const std::shared_ptr<const Daybreak::ITexture2d>& texture,
            unsigned int slotIndex) override;

        // Bind vertex buffer to pipeline for rendering.
        virtual void bindVertexBuffer(const std::shared_ptr<const Daybreak::VertexBuffer>& vertexBuffer) override;

    // Shader variables.
    public:
        // Set int shader parameter.
        virtual void setShaderInt(const ShaderVariable& v, int value) override;

        // Set float shader parameter.
        virtual void setShaderFloat(const ShaderVariable& v, float value) override;

        // Set bool shader parameter.
        virtual void setShaderBool(const ShaderVariable& v, bool value) override;

        // Set float2 shader parameter.
        virtual void setShaderVector2f(const ShaderVariable& v, float x, float y) override;

        // Set float2 shader parameter.
        virtual void setShaderVector2f(const ShaderVariable& v, const glm::vec2& vec) override;

        // Set float3 shader parameter.
        virtual void setShaderVector3f(const ShaderVariable& v, float x, float y, float z) override;

        // Set float3 shader parameter.
        virtual void setShaderVector3f(const ShaderVariable& v, const glm::vec3& vec) override;

        // Set float4 shader parameter.
        virtual void setShaderVector4f(const ShaderVariable& v, float x, float y, float z, float w) override;

        // Set float4 shader parameter.
        virtual void setShaderVector4f(const ShaderVariable& v, const glm::vec4& vec) override;

        // Set matrix4x4 shader parameter.
        virtual void setShaderMatrix3(const ShaderVariable& v, const glm::mat3& vec) override;

        // Set matrix4x4 shader parameter.
        virtual void setShaderMatrix4(const ShaderVariable& v, const glm::mat4& vec) override;

    // Contextual commands.
    public:
        virtual void drawTriangles(unsigned int offset, unsigned int count) override;

    // Properties.
    public:
        // Enable or disable depth testing.
        virtual void setDepthTestEnabled(bool isEnabled) override;

        // Check if depth testing is enabled.
        virtual bool isDepthTestEnabled() const override;

        // Enable or disable wireframe rendering.
        virtual void setWireframeEnabled(bool isEnabled) override;

        // Check if wireframe rendering is enabled.
        virtual bool isWireframeEnabled() const override;

    private:
        // Releases allocated resources (not sure if needed).
        void destroy();

    private:
        bool m_depthTestEnabled = false;        // TODO: Find out what is OpenGL default.
        bool m_wireframeEnabled = false;        // TODO: Find out what is OpenGL default.
        
        IndexElementType m_currentIndexElementType;
        size_t m_currentIndexElementCount;
    };
}
