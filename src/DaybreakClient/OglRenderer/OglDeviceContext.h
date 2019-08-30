#pragma once
#include "Renderer\DeviceContext.h"
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    /** OpenGL implementation of DeviceContext. */
    class OglDeviceContext final : public IDeviceContext
    {
    public:
        /** Initialize device context to default state. */
        OglDeviceContext();

        /** Destructor. */
        ~OglDeviceContext();

    public:
        /** Create a new 2d texture. */
        virtual std::unique_ptr<ITexture2d> createTexture2d(
            const Image& image,                                     ///< Source image.
            const TextureParameters& params) override;              ///< Texture sampling and generation parameters.

        /** Create a new index buffer. */
        virtual std::unique_ptr<IndexBuffer> createIndexBuffer(
            const MeshData& mesh) override;                         ///< Source index data.
       
        /** Create a new input layout descriptor. */
        virtual std::unique_ptr<InputLayout> createInputLayout(
            const InputLayoutDescription& layoutDescription) override;

        /** Create a new shader program. */
        virtual std::unique_ptr<IShader> compileShader(
            const std::string& shaderName,                          ///< Name of the shader program.
            std::string_view vertexShaderProgram,                   ///< Vertex shader code.
            std::string_view fragmentShaderProgram) override;       ///< Pixel shader code.

        /** Create a new vertex buffer. */
        virtual std::unique_ptr<VertexBuffer> createVertexBuffer(
            const MeshData& mesh) override;             ///< Source vertex data.

    protected:
        /** Destroy allocated resources. */
        void destroy();
    };
}