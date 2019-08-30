#pragma once
#include "Renderer/InputLayout.h"
#include "Graphics/InputLayoutDescription.h"
#include <glad\glad.h>
#include <memory>
#include <vector>

namespace Daybreak
{
    class IRenderContext;
    class VertexBuffer;
}

namespace Daybreak::OpenGlRenderer
{
    class OglRenderContext;
    class OglVertexBuffer;

    // OpenGL vertex array object encapsulation.
    class OglInputLayout final : public InputLayout
    {
    public:
        // Constructor.
        OglInputLayout(GLuint vao, const InputLayoutDescription& layoutDescription);

        // Destructor.
        virtual ~OglInputLayout();

        // Get vao object.
        GLuint vao() const noexcept { return m_vao; }

        // Set vao object.
        void setVAO(GLuint vao);

        // Create a new VAO object.
        static std::unique_ptr<OglInputLayout> generate(const InputLayoutDescription& layoutDescription);

    private:
        // Destroy VAO.
        void destroy();

    private:
        GLuint m_vao = 0;
    };
}