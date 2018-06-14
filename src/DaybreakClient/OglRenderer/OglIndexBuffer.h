#pragma once
#include "Renderer/IndexBuffer.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL index buffer encapsulation.
    class OglIndexBuffer final : public IIndexBuffer
    {
    public:
        // Constructor.
        explicit OglIndexBuffer(GLuint ebo = 0);

        // Destructor.
        virtual ~OglIndexBuffer();

        // Make index buffer active.
        virtual void bind() override;

        // Get index buffer object.
        GLuint ebo() const noexcept { return m_ebo; }

        // Set index buffer object.
        void setEBO(GLuint ebo);

        // Create a new ebo object.
        static std::unique_ptr<OglIndexBuffer> generate(   // TODO: Improve interface with IMesh + IInputLayout
            void * indexBufferData,
            size_t elementSizeInBytes,
            size_t elementCount);

    private:
        void destroy();

    private:
        GLuint m_ebo = 0;
    };
}