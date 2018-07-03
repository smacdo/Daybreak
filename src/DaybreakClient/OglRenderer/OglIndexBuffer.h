#pragma once
#include "Renderer/IndexBuffer.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL index buffer encapsulation.
    class OglIndexBuffer final : public IndexBuffer
    {
    public:
        // TODO: Default constructor?

        // Constructor.
        OglIndexBuffer(
            _In_ IndexElementType elementType,
            _In_ size_t elementCount,
            _In_ GLuint ebo);

        // Destructor.
        virtual ~OglIndexBuffer();

    public:
        // Get the number of elements in the buffer.
        virtual size_t elementCount() const noexcept override { return m_elementCount; }

        // Get the type of elments in the buffer.
        virtual IndexElementType elementType() const noexcept override { return m_elementType; }

        // Get elmeent buffer object.
        GLuint ebo() const noexcept { return m_ebo; }

        // Set index buffer object.
        void setEBO(GLuint ebo);

        // Create a new ebo object.
        static std::unique_ptr<OglIndexBuffer> generate(   // TODO: Improve interface with IMesh + IInputLayout
            _In_ IndexElementType elementType,
            _In_ size_t elementCount,
            _In_ void * indexBufferData);

    private:
        void destroy();

    private:
        IndexElementType m_elementType;
        size_t m_elementCount;

        GLuint m_ebo = 0;
    };
}