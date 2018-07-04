#include "stdafx.h"
#include "OglIndexBuffer.h"
#include "OglError.h"
#include "Graphics/Mesh/MeshData.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglIndexBuffer::OglIndexBuffer(
    _In_ IndexElementType elementType,
    _In_ size_t elementCount,
    _In_ GLuint ebo)
    : m_elementType(elementType),
      m_elementCount(elementCount),
      m_ebo(ebo)
{
    // Only set ebo if constructor was initalized with a non-zero value.
    //  TODO: Can we assert if zero and have default constructor?
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
std::unique_ptr<OglIndexBuffer> OglIndexBuffer::generate(_In_ const MeshData& mesh)
{
    CHECK_NOT_ZERO(mesh.indexCount());
    CHECK_NOT_ZERO(mesh.indexElementSizeInBytes());

    // Create the index buffer object.
    GLuint ebo = 0;

    glGenBuffers(1, &ebo);
    glCheckForErrors();

    EXPECT(ebo != 0, "OpenGL ebo object must have non-zero id");

    // Bind the index buffer object prior to copying index data into the buffer.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCheckForErrors();

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh.indexElementSizeInBytes() * mesh.indexCount(),
        mesh.rawIndexBufferData(),
        GL_STATIC_DRAW);
    glCheckForErrors();

    return std::make_unique<OglIndexBuffer>(mesh.indexElementType(), mesh.indexCount(), ebo);
}
