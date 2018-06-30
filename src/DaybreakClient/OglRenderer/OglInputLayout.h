#pragma once
#include "Renderer/InputLayout.h"
#include <glad\glad.h>
#include <memory>
#include <vector>

namespace Daybreak
{
    class IRenderContext;
    class IVertexBuffer;
}

namespace Daybreak::OpenGlRenderer
{
    class OglRenderContext;
    class OglVertexBuffer;

    // OpenGL vertex array object encapsulation.
    class OglInputLayout final : public IInputLayout
    {
    public:
        // Constructor.
        OglInputLayout(GLuint vao, const std::vector<attribute_t>& attributes);

        // Destructor.
        virtual ~OglInputLayout();

        // Get attribute count (number of individual elements in layout).
        virtual size_t attributeCount() const noexcept override;

        // Get an attribute by index.
        virtual attribute_t getAttributeByIndex(unsigned int index) const noexcept override;

        // Get vao object.
        GLuint vao() const noexcept { return m_vao; }

        // Set vao object.
        void setVAO(GLuint vao);

        // Create a new VAO object.
        //  TODO: Move this to OglRenderDevice::createInputLayout.
        static std::unique_ptr<OglInputLayout> generate(
            const std::vector<attribute_t>& attributes,
            const std::shared_ptr<Daybreak::IRenderContext>& renderContext,
            const std::shared_ptr<const Daybreak::IVertexBuffer>& vertexBuffer);

    private:
        // Destroy VAO.
        void destroy();

    private:
        std::vector<attribute_t> m_attributes;
        GLuint m_vao = 0;
    };
}