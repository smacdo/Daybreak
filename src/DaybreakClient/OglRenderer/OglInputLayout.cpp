#include "stdafx.h"
#include "OglInputLayout.h"
#include "OglError.h"
#include "OglRenderContext.h"
#include "OglVertexBuffer.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
namespace
{
    GLenum ToGlElementType(InputAttribute::StorageType type)
    {
        switch (type)
        {
        case InputAttribute::StorageType::Byte:
            return GL_BYTE;
        case InputAttribute::StorageType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case InputAttribute::StorageType::Short:
            return GL_SHORT;
        case InputAttribute::StorageType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case InputAttribute::StorageType::Int:
            return GL_INT;
        case InputAttribute::StorageType::UnsignedInt:
            return GL_UNSIGNED_INT;
        case InputAttribute::StorageType::HalfFloat:
            return GL_HALF_FLOAT;
        case InputAttribute::StorageType::Float:
            return GL_FLOAT;
        case InputAttribute::StorageType::Double:
            return GL_DOUBLE;
        default:
            throw std::runtime_error("unknwon enum");
        }
    }

    GLsizei GetElementSize(InputAttribute::StorageType type)
    {
        switch (type)
        {
        case InputAttribute::StorageType::Byte:
            return 1;;
        case InputAttribute::StorageType::UnsignedByte:
            return 1;
        case InputAttribute::StorageType::Short:
            return 2;
        case InputAttribute::StorageType::UnsignedShort:
            return 2;
        case InputAttribute::StorageType::Int:
            return 4;
        case InputAttribute::StorageType::UnsignedInt:
            return 4;
        case InputAttribute::StorageType::HalfFloat:
            return 2;
        case InputAttribute::StorageType::Float:
            return 4;
        case InputAttribute::StorageType::Double:
            return 8;
        default:
            throw std::runtime_error("unknwon enum");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglInputLayout::OglInputLayout(GLuint vao, std::shared_ptr<const InputLayoutDescription> layoutDescription)
    : InputLayout(layoutDescription),
      m_vao(vao)
{
    // TODO: Only calculate this once.
    //  (When code is refactored so that this class is instantiated by OglRenderDevice it should be calculated there
    //   and passed here as a parameter).
    GLint maxAttributeCount = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributeCount);

    EXPECT(maxAttributeCount > 0, "Maximum attribute count must be larger than zero");
    EXPECT(layoutDescriptionRef().attributeCount() < (size_t)maxAttributeCount, "Must be smaller than OpenGL max");

    // Only set VAO if constructor was initalized with a non-zero value.
    EXPECT(vao != 0, "OpenGL input layouts should always be initialized to a valid VAO object");

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
std::unique_ptr<OglInputLayout> OglInputLayout::generate(
    std::shared_ptr<const InputLayoutDescription> layoutDescription,
    const std::shared_ptr<Daybreak::IRenderContext>& renderContext,     // TODO: Remove.
    const std::shared_ptr<const Daybreak::VertexBuffer>& vertexBuffer)
{
    CHECK_NOT_NULL(layoutDescription);
    CHECK_NOT_NULL(renderContext);
    CHECK_NOT_NULL(vertexBuffer);

    // Create the vertex array object.
    GLuint vao = 0;

    glGenVertexArrays(1, &vao);
    glCheckForErrors();

    EXPECT(vao != 0, "OpenGL VAO object must have non-zero id");

    // Bind the vertex array object once to establish the object as a VAO (otherwise glIsVertexArray returns false!).
    //  TODO: Use render context to enable the VAO layout rather than directly here.
    glBindVertexArray(vao);
    glCheckForErrors();

    // TODO: Only calculate this once.
    //  (When code is refactored so that this class is instantiated by OglRenderDevice it should be calculated there
    //   and passed here as a parameter).
    GLint maxAttributeCount = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributeCount);

    EXPECT(maxAttributeCount > 0, "Maximum attribute count must be larger than zero");

    // Throw an exception if the attribute array is empty.
    if (layoutDescription->attributeCount() < 1)
    {
        throw std::runtime_error("Input layout must have at least one attribute defined");
    }
    
    // Disable all vertex attribute slots prior to enabling some them.
    //  TODO: This can be optimized by remembering which slots were enabled or disabled.
    for (size_t i = 0; i < static_cast<size_t>(maxAttributeCount); ++i)
    {
        glDisableVertexAttribArray(static_cast<GLuint>(i));
    }

    // Define all slot attributes in the OpenGL vertex array object.
    auto totalSize = static_cast<GLsizei>(layoutDescription->elementSizeInBytes());
    uintptr_t attributeOffset = 0;

    for (GLsizei i = 0; i < layoutDescription->attributeCount(); ++i)
    {
        const auto& attribute = layoutDescription->getAttributeByIndex(i);

        glVertexAttribPointer(
            static_cast<GLuint>(i),
            attribute.count,
            ToGlElementType(attribute.type),
            GL_FALSE,
            totalSize,
            reinterpret_cast<void*>(attributeOffset));
        glCheckForErrors();

        glEnableVertexAttribArray(static_cast<GLuint>(i));
        glCheckForErrors();

        attributeOffset += attribute.count * GetElementSize(attribute.type);
    }

    return std::make_unique<OglInputLayout>(vao, layoutDescription);
}
