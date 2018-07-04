#pragma once
#include "Renderer/VertexBuffer.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak
{
    class MeshData;
}

namespace Daybreak::OpenGlRenderer
{
    // OpenGL vertex buffer encapsulation.
    class OglVertexBuffer final : public VertexBuffer
    {
    public:
        // Constructor.
        OglVertexBuffer(size_t elementCount, GLuint vbo = 0);

        // Destructor.
        virtual ~OglVertexBuffer();

        // Get the number of elements in the buffer.
        virtual size_t elementCount() const noexcept override;

        // Get vertex buffer object.
        GLuint vbo() const noexcept { return m_vbo; }

        // Set vertex buffer object.
        void setVBO(GLuint vbo);

        // Create a new VBO object.
        static std::unique_ptr<OglVertexBuffer> generate(const MeshData& mesh);

    private:
        void destroy();

    private:
        GLuint m_vbo = 0;
        size_t m_elementCount = 0;
    };
}
