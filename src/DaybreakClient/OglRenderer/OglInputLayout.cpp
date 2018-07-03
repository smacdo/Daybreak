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
    GLenum ToGlElementType(InputLayout::ElementType type)
    {
        switch (type)
        {
        case InputLayout::ElementType::Byte:
            return GL_BYTE;
        case InputLayout::ElementType::UnsignedByte:
            return GL_UNSIGNED_BYTE;
        case InputLayout::ElementType::Short:
            return GL_SHORT;
        case InputLayout::ElementType::UnsignedShort:
            return GL_UNSIGNED_SHORT;
        case InputLayout::ElementType::Int:
            return GL_INT;
        case InputLayout::ElementType::UnsignedInt:
            return GL_UNSIGNED_INT;
        case InputLayout::ElementType::HalfFloat:
            return GL_HALF_FLOAT;
        case InputLayout::ElementType::Float:
            return GL_FLOAT;
        case InputLayout::ElementType::Double:
            return GL_DOUBLE;
        default:
            throw std::runtime_error("unknwon enum");
        }
    }

    GLsizei GetElementSize(InputLayout::ElementType type)
    {
        switch (type)
        {
        case InputLayout::ElementType::Byte:
            return 1;;
        case InputLayout::ElementType::UnsignedByte:
            return 1;
        case InputLayout::ElementType::Short:
            return 2;
        case InputLayout::ElementType::UnsignedShort:
            return 2;
        case InputLayout::ElementType::Int:
            return 4;
        case InputLayout::ElementType::UnsignedInt:
            return 4;
        case InputLayout::ElementType::HalfFloat:
            return 2;
        case InputLayout::ElementType::Float:
            return 4;
        case InputLayout::ElementType::Double:
            return 8;
        default:
            throw std::runtime_error("unknwon enum");
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglInputLayout::OglInputLayout(GLuint vao, const std::vector<attribute_t>& attributes)
    : m_attributes(attributes),
      m_vao(vao)
{
    // TODO: Only calculate this once.
    //  (When code is refactored so that this class is instantiated by OglRenderDevice it should be calculated there
    //   and passed here as a parameter).
    GLint maxAttributeCount = 0;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributeCount);

    EXPECT(maxAttributeCount > 0, "Maximum attribute count must be larger than zero");
    EXPECT(attributes.size() < maxAttributeCount, "Attribute count must be smaller than OpenGL max");

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
size_t OglInputLayout::attributeCount() const noexcept
{
    return m_attributes.size();
}

//---------------------------------------------------------------------------------------------------------------------
InputLayout::attribute_t OglInputLayout::getAttributeByIndex(unsigned int index) const noexcept
{
    return m_attributes[index];
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
    const std::vector<attribute_t>& attributes,
    const std::shared_ptr<Daybreak::IRenderContext>& renderContext,     // TODO: Remove.
    const std::shared_ptr<const Daybreak::VertexBuffer>& vertexBuffer)
{
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
    if (attributes.size() < 1)
    {
        throw std::runtime_error("Input layout must have at least one attribute defined");
    }

    // Calculate the size of all the attributes. Also check that slots are defined in order.
    GLsizei totalSize = 0;
    auto lastSlot = attributes[attributes.size() - 1].slot;

    for (size_t i = 0; i < attributes.size(); ++i)
    {
        // Ensure that the slots are defined in order.
        if (i > 0 && attributes[i].slot <= attributes[i - 1].slot)
        {
            throw std::runtime_error("Attribute slot must be larger than previous slot");
        }

        // Add size to total.
        totalSize += attributes[i].count * GetElementSize(attributes[i].type);
    }

    // Disable all vertex attribute slots prior to enabling some them.
    //  TODO: This can be optimized by remembering which slots were enabled or disabled.
    for (size_t i = 0; i < maxAttributeCount; ++i)
    {
        glDisableVertexAttribArray(static_cast<GLuint>(i));
    }

    // Define all slot attributes in the OpenGL vertex array object.
    uintptr_t attributeOffset = 0;

    for (size_t i = 0; i < attributes.size(); ++i)
    {
        glVertexAttribPointer(
            attributes[i].slot,
            attributes[i].count, 
            ToGlElementType(attributes[i].type),
            attributes[i].shouldNormalize ? GL_TRUE : GL_FALSE,
            totalSize,
            reinterpret_cast<void*>(attributeOffset));
        glCheckForErrors();

        glEnableVertexAttribArray(attributes[i].slot);
        glCheckForErrors();

        attributeOffset += attributes[i].count * GetElementSize(attributes[i].type);
    }

    return std::make_unique<OglInputLayout>(vao, attributes);
}
