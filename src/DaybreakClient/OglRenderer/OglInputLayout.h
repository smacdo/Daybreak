#pragma once
#include "Renderer/InputLayout.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL vertex array object encapsulation.
    class OglInputLayout : public IInputLayout
    {
    public:
        // Constructor.
        OglInputLayout(GLuint vao = 0);

        // Destructor.
        virtual ~OglInputLayout();

        // Bind input layout.
        virtual void bind() override;

        // Get vao object.
        GLuint vao() const noexcept { return m_vao; }

        // Set vao object.
        void setVAO(GLuint vao);

        // Create a new VAO object.
        static std::unique_ptr<OglInputLayout> Generate();

    private:
        GLuint m_vao = 0;
    };
}