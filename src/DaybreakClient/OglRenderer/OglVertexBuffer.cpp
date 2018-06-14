#include "stdafx.h"
#include "OglVertexBuffer.h"
#include "OglError.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglVertexBuffer::OglVertexBuffer(GLuint vbo)
    : IVertexBuffer(),
      m_vbo(vbo)
{
    // Only set VBO if constructor was initalized with a non-zero value.
    if (m_vbo != 0)
    {
        setVBO(m_vbo);
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglVertexBuffer::~OglVertexBuffer()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglVertexBuffer::destroy()
{
    if (glIsBuffer(m_vbo) == GL_TRUE)
    {
        GLuint vbo[] = { m_vbo };
        glDeleteBuffers(1, std::begin(vbo));

        m_vbo = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglVertexBuffer::bind()
{
    // TODO: Delete and remove IBindable base.
}

//---------------------------------------------------------------------------------------------------------------------
void OglVertexBuffer::setVBO(GLuint vbo)
{
    if (glIsBuffer(vbo) == GL_FALSE)
    {
        // TODO: Better exception.
        throw std::runtime_error("Vertex buffer object not valid");
    }

    m_vbo = vbo;
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglVertexBuffer> OglVertexBuffer::generate(
    void * vertexBufferData,
    size_t elementSizeInBytes,
    size_t elementCount)
{
    CHECK_NOT_NULL(vertexBufferData);
    CHECK_NOT_ZERO(elementSizeInBytes);
    CHECK_NOT_ZERO(elementCount);

    GLuint vbo = 0;

    // Create the vertex buffer object.
    glGenBuffers(1, &vbo);
    glCheckForErrors();

    EXPECT(vbo != 0, "OpenGL VBO object must have non-zero id");

    // Bind the vertex buffer object prior to copying vertex data into the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(
        GL_ARRAY_BUFFER,
        elementSizeInBytes * elementCount,
        vertexBufferData,
        GL_STATIC_DRAW);
    glCheckForErrors();

    return std::make_unique<OglVertexBuffer>(vbo);
}
