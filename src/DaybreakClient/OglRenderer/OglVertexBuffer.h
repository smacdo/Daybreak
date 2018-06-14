#pragma once
#include "Renderer/VertexBuffer.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL vertex buffer encapsulation.
    class OglVertexBuffer final : public IVertexBuffer
    {
    public:
        // Constructor.
        explicit OglVertexBuffer(GLuint vbo = 0);

        // Destructor.
        virtual ~OglVertexBuffer();

        // Make vertex buffer active.
        virtual void bind() override;

        // Get vertex buffer object.
        GLuint vbo() const noexcept { return m_vbo; }

        // Set vertex buffer object.
        void setVBO(GLuint vbo);

        // Create a new VBO object.
        static std::unique_ptr<OglVertexBuffer> generate(   // TODO: Improve interface with IMesh + IInputLayout
            void * vertexBufferData,
            size_t elementSizeInBytes,
            size_t elementCount);

    private:
        void destroy();

    private:
        GLuint m_vbo = 0;
    };
}