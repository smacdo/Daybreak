#include "stdafx.h"
#include "OglIndexBuffer.h"
#include "OglError.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglIndexBuffer::OglIndexBuffer(GLuint ebo)
    : IIndexBuffer(),
      m_ebo(ebo)
{
    // Only set ebo if constructor was initalized with a non-zero value.
    if (m_ebo != 0)
    {
        setEBO(m_ebo);
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglIndexBuffer::~OglIndexBuffer()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglIndexBuffer::destroy()
{
    if (glIsBuffer(m_ebo) == GL_TRUE)
    {
        GLuint ebo[] = { m_ebo };
        glDeleteBuffers(1, std::begin(ebo));

        m_ebo = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglIndexBuffer::bind()
{
    // TODO: Delete and remove IBindable base.
}

//---------------------------------------------------------------------------------------------------------------------
void OglIndexBuffer::setEBO(GLuint ebo)
{
    if (glIsBuffer(ebo) == GL_FALSE)
    {
        // TODO: Better exception.
        throw std::runtime_error("Index buffer object not valid");
    }

    m_ebo = ebo;
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglIndexBuffer> OglIndexBuffer::generate(
    void * indexBufferData,
    size_t elementSizeInBytes,
    size_t elementCount)
{
    CHECK_NOT_NULL(indexBufferData);
    CHECK_NOT_ZERO(elementSizeInBytes);
    CHECK_NOT_ZERO(elementCount);

    GLuint ebo = 0;

    // Create the index buffer object.
    glGenBuffers(1, &ebo);
    glCheckForErrors();

    EXPECT(ebo != 0, "OpenGL ebo object must have non-zero id");

    // Bind the index buffer object prior to copying index data into the buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCheckForErrors();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        elementSizeInBytes * elementCount,
        indexBufferData,
        GL_STATIC_DRAW);
    glCheckForErrors();

    return std::make_unique<OglIndexBuffer>(ebo);
}
