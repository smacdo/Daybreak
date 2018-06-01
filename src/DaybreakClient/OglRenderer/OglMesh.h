#pragma once
#include "Renderer/IMesh.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL mesh.
 /*   class OglMesh : public IMesh
    {
    public:
        // Constructor.
        OglMesh(GLUint vbo, GLuin);

        // Destructor.
        virtual ~OglMesh();

        // Bind input layout.
        virtual void bind() override;

        // Get vao object.
        GLuint vao() const noexcept { return m_vao; }

        // Set vao object.
        void setVAO(GLuint vao);

        // Create a new VAO object.
        static std::unique_ptr<OglMesh> Generate();

    private:
        GLuint m_vao = 0;
    };*/
}