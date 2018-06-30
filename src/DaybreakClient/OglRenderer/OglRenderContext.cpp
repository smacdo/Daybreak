#include "stdafx.h"
#include "OglRenderContext.h"
#include "OglError.h"
#include "OglVertexBuffer.h"
#include "OglIndexBuffer.h"
#include "OglInputLayout.h"
#include "OglShader.h"
#include "OglTexture.h"

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglRenderContext::OglRenderContext()
    : IRenderContext()
{
}

//---------------------------------------------------------------------------------------------------------------------
OglRenderContext::~OglRenderContext()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::clearColorAndDepthBuffers()
{
    // TODO: Take flags.
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::bindIndexBuffer(const std::shared_ptr<const IIndexBuffer>& indexBuffer)
{
    CHECK_NOT_NULL(indexBuffer);

    // TODO: Optimize by not changing if already bound.
    auto ib = static_cast<const OglIndexBuffer*>(indexBuffer.get());

    if (ib->ebo() == 0)
    {
        throw std::runtime_error("Index buffer was not assigned before being bound");
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->ebo());
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::bindInputLayout(const std::shared_ptr<const IInputLayout>& inputLayout)
{
    CHECK_NOT_NULL(inputLayout);

    // TODO: Optimize by not changing if already bound.
    auto glInputLayout = static_cast<const OglInputLayout*>(inputLayout.get());

    if (glInputLayout->vao() == 0)
    {
        throw std::runtime_error("Vertex array object was not assigned before being bound");
    }

    glBindVertexArray(glInputLayout->vao());
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::bindShader(const std::shared_ptr<const IShader>& shader)
{
    CHECK_NOT_NULL(shader);

    // TODO: Optimize by not changing if already bound.
    auto glShader = static_cast<const OglShader*>(shader.get());

    if (glShader->shaderProgram() == 0)
    {
        throw std::runtime_error("Shader was not assigned before being bound");
    }

    glUseProgram(glShader->shaderProgram());
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::bindTexture(
    const std::shared_ptr<const ITexture2d>& texture,
    unsigned int slotIndex)
{
    CHECK_NOT_NULL(texture);

    // TODO: Test at runtime max number of slots and check that this slot is valid. Throw exception not assert.
    EXPECT(slotIndex < 16, "Must be a valid texture unit slot");
    
    // TODO: Optimize by not changing if already bound.
    auto glTexture = static_cast<const OglTexture2d*>(texture.get());

    if (glTexture->textureId() == 0)
    {
        throw std::runtime_error("Texture was not assigned before being bound");
    }

    glActiveTexture(GL_TEXTURE0 + slotIndex);
    glCheckForErrors();

    glBindTexture(GL_TEXTURE_2D, glTexture->textureId());
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::bindVertexBuffer(const std::shared_ptr<const IVertexBuffer>& vertexBuffer)
{
    CHECK_NOT_NULL(vertexBuffer);

    // TODO: Optimize by not changing if already bound.
    auto vb = static_cast<const OglVertexBuffer*>(vertexBuffer.get());

    if (vb->vbo() == 0)
    {
        throw std::runtime_error("Vertex buffer was not assigned before being bound");
    }

    glBindBuffer(GL_ARRAY_BUFFER, vb->vbo());
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::drawTriangles(unsigned int offset, unsigned int count)
{
    // TODO: Check offset, count bounds against currently bound index buffer (+vertex buffer).
    // TODO: Check that vertex, index everything bound as expected.
    glDrawArrays(GL_TRIANGLES, offset, count);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setDepthTestEnabled(bool isEnabled)
{
    // TODO: Consider optimizing by not setting if mode does not change.
    if (isEnabled)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    m_depthTestEnabled = isEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
bool OglRenderContext::isDepthTestEnabled() const
{
    // TODO: In debug mode test if flag matches OpenGL state.
    return m_depthTestEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setWireframeEnabled(bool isEnabled)
{
    // TODO: Consider optimizing by not setting if mode does not change.
    glPolygonMode(GL_FRONT_AND_BACK, isEnabled ? GL_LINE : GL_FILL);
    m_wireframeEnabled = isEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
bool OglRenderContext::isWireframeEnabled() const
{
    // TODO: In debug mode test if flag matches OpenGL state.
    return m_wireframeEnabled;
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderInt(const ShaderVariable& v, int value)
{
    glUniform1i(v.address(), value);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderFloat(const ShaderVariable& v, float value)
{
    glUniform1f(v.address(), value);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderBool(const ShaderVariable& v, bool value)
{
    glUniform1i(v.address(), static_cast<int>(value));
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector2f(const ShaderVariable& v, float x, float y)
{
    glUniform2f(v.address(), x, y);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector2f(const ShaderVariable& v, const glm::vec2& vec)
{
    glUniform2f(v.address(), vec.x, vec.y);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector3f(const ShaderVariable& v, float x, float y, float z)
{
    glUniform3f(v.address(), x, y, z);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector3f(const ShaderVariable& v, const glm::vec3& vec)
{
    glUniform3f(v.address(), vec.x, vec.y, vec.z);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector4f(const ShaderVariable& v, float x, float y, float z, float w)
{
    glUniform4f(v.address(), x, y, z, w);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderVector4f(const ShaderVariable& v, const glm::vec4& vec)
{
    glUniform4f(v.address(), vec.x, vec.y, vec.z, vec.w);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderMatrix3(const ShaderVariable& v, const glm::mat3& vec)
{
    glUniformMatrix3fv(v.address(), 1, GL_FALSE, glm::value_ptr(vec));
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::setShaderMatrix4(const ShaderVariable& v, const glm::mat4& vec)
{
    glUniformMatrix4fv(v.address(), 1, GL_FALSE, glm::value_ptr(vec));
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::destroy()
{
    // TODO: Not sure if needed.
}
