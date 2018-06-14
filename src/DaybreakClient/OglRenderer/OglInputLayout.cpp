#include "stdafx.h"
#include "OglInputLayout.h"
#include "OglError.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglInputLayout::OglInputLayout(GLuint vao)
    : IInputLayout(),
      m_vao(vao)
{
    // Only set VAO if constructor was initalized with a non-zero value.
    if (m_vao != 0)
    {
        setVAO(m_vao);
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglInputLayout::~OglInputLayout()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglInputLayout::destroy()
{
    if (glIsVertexArray(m_vao) == GL_TRUE)
    {
        GLuint vao[] = { m_vao };
        glDeleteVertexArrays(1, std::begin(vao));

        m_vao = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglInputLayout::bind()
{
    // TODO: Delete and remove IBindable base.
}

//---------------------------------------------------------------------------------------------------------------------
void OglInputLayout::setVAO(GLuint vao)
{
    if (glIsVertexArray(vao) == GL_FALSE)
    {
        // TODO: Better exception.
        throw std::runtime_error("vao object not valid");
    }

    m_vao = vao;
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglInputLayout> OglInputLayout::Generate()
{
    GLuint vao = 0;

    // Create the vertex array object.
    glGenVertexArrays(1, &vao);
    glCheckForErrors();

    EXPECT(vao != 0, "OpenGL VAO object must have non-zero id");

    // Bind the vertex array object once to establish the object as a VAO (otherwise glIsVertexArray returns false!).
    glBindVertexArray(vao);
    glCheckForErrors();

    return std::make_unique<OglInputLayout>(vao);
}
