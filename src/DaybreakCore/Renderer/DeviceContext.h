#pragma once
#include <memory>

#include "Renderer\Texture.h"

namespace Daybreak
{
    class Image;
    class IndexBuffer;
    class InputLayout;
    class InputLayoutDescription;
    class MeshData;
    class IShader;
    class VertexBuffer;

    /*** Hardware independent API for managing rendering resources. */
    class IDeviceContext
    {
    public:
        /** Initialize device context to default state. */
        IDeviceContext() = default;

        /** Copy constructor (deleted). */
        IDeviceContext(const IDeviceContext&) = delete;

        /** Destructor. */
        virtual ~IDeviceContext() = default;

        /** Assignment operator (deleted). */
        IDeviceContext& operator =(const IDeviceContext&) = delete;

    public:
        /** Create a new 2d texture. */
        virtual std::unique_ptr<ITexture2d> createTexture2d(
            const Image& image,                     ///< Source image.
            const TextureParameters& params) = 0;   ///< Texture sampling and generation parameters.

        /** Create a new index buffer. */
        virtual std::unique_ptr<IndexBuffer> createIndexBuffer(
            const MeshData& mesh) = 0;              ///< Source index data.

        /** Create a new input layout descriptor. */
        virtual std::unique_ptr<InputLayout> createInputLayout(
            const InputLayoutDescription& layoutDescription) = 0;

        /** Create a new shader program. */
        virtual std::unique_ptr<IShader> compileShader(
            const std::string& shaderName,                          ///< Name of the shader program.
            std::string_view vertexShaderProgram,                   ///< Vertex shader code.
            std::string_view fragmentShaderProgram) = 0;            ///< Pixel shader code.

        /** Create a new vertex buffer. */
        virtual std::unique_ptr<VertexBuffer> createVertexBuffer(
            const MeshData& mesh) = 0;              ///< Source vertex data.
    };
}