#pragma once
#include "Graphics/InputLayout.h"
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
        OglInputLayout(GLuint vao, const std::vector<InputAttribute>& attributes);

        // Destructor.
        virtual ~OglInputLayout();

        // Get vao object.
        GLuint vao() const noexcept { return m_vao; }

        // Set vao object.
        void setVAO(GLuint vao);

        // Create a new VAO object.
        //  TODO: Move this to OglRenderDevice::createInputLayout.
        static std::unique_ptr<OglInputLayout> generate(
            const std::vector<InputAttribute>& attributes,
            const std::shared_ptr<Daybreak::IRenderContext>& renderContext,
            const std::shared_ptr<const Daybreak::VertexBuffer>& vertexBuffer);

    private:
        // Destroy VAO.
        void destroy();

    private:
        GLuint m_vao = 0;
    };
}