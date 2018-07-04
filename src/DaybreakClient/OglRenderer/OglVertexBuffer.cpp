#include "stdafx.h"
#include "OglVertexBuffer.h"
#include "OglError.h"
#include "Graphics/Mesh/MeshData.h"
#include "Graphics/Mesh/VertexFormat.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglVertexBuffer::OglVertexBuffer(size_t elementCount, GLuint vbo)
    : VertexBuffer(),
      m_elementCount(elementCount),
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
size_t OglVertexBuffer::elementCount() const noexcept
{
    return m_elementCount;
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
std::unique_ptr<OglVertexBuffer> OglVertexBuffer::generate(const MeshData& mesh)
{
    CHECK_NOT_ZERO(mesh.vertexCount());
    CHECK_NOT_ZERO(mesh.vertexElementSizeInBytes());

    // Create the vertex buffer object.
    GLuint vbo = 0;

    glGenBuffers(1, &vbo);
    glCheckForErrors();

    EXPECT(vbo != 0, "OpenGL VBO object must have non-zero id");

    // Bind the vertex buffer object prior to copying vertex data into the buffer.
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(
        GL_ARRAY_BUFFER,
        mesh.vertexElementSizeInBytes() * mesh.vertexCount(),
        mesh.rawVertexBufferData(),
        GL_STATIC_DRAW);
    glCheckForErrors();

    return std::make_unique<OglVertexBuffer>(mesh.vertexCount(), vbo);
}
