#include "stdafx.h"
#include "SceneRenderer.h"
#include "Scene.h"
#include "Shader.h"
#include "ErrorHandling.h"

#include <glad\glad.h>
#include <string>
#include <memory>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------------
// TODO: Move this to another file.
namespace
{
    const char * StandardVertexShaderSource =
        "#version 330 core\n"
        "layout (location = 0) in vec3 posIn;\n"
        "void main()\n"
        "{\n"
        "    gl_Position = vec4(posIn.x, posIn.y, posIn.z, 1.0);\n"
        "}\n";

    const char * StandardFragmentShaderSource =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\n";
}

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::SceneRenderer()
{
    CreateDefaultScene();
}

//---------------------------------------------------------------------------------------------------------------------
SceneRenderer::~SceneRenderer()
{
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::Render(const Scene& scene)
{
    // Enable wireframe rendering if requested otherwise use solid mode.
    glPolygonMode(GL_FRONT_AND_BACK, IsWireframeEnabled() ? GL_LINE : GL_FILL);

    m_standardShader->Activate();
    glBindVertexArray(m_standardVAO);

    // Draw rect.
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Unbind vertex array.
    glBindVertexArray(0);

    // Make sure no errors happened while drawing.
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    glGenVertexArrays(1, &m_standardVAO);
    glBindVertexArray(m_standardVAO);

    // Create a simple rectangle to render.
    // TODO: Remove this once scene loading is added.
    const float DefaultVertices[] =
    {
         0.5f,  0.5f, 0.0f,     // Top right.
         0.5f, -0.5f, 0.0f,     // Bottom right.
        -0.5f, -0.5f, 0.0f,     // Bottom left.
        -0.5f,  0.5f, 0.0f,     // Top left.
    };

    const unsigned int DefaultIndices[] =
    {
        0, 1, 3,    // Left triangle.
        1, 2, 3     // Right triangle.
    };

    // Upload vertex data.
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultVertices), DefaultVertices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Upload index data.
    unsigned int ebo = 0;
    glGenBuffers(1, &ebo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glCheckForErrors();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(DefaultIndices), DefaultIndices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Construct standard shader.
    m_standardShader = std::move(
        Shader::LoadFromString(
            "StandardShader",
            "StandardVertexShader.glsl",
            StandardVertexShaderSource,
            "StandardFragmentShader.glsl",
            StandardFragmentShaderSource));
    m_standardShader->Activate();

    // Generate vertex attributes for the standard shader.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glCheckForErrors();

    glEnableVertexAttribArray(0);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::VerifyShaderLinked(unsigned int program, const std::string& programName)
{
    int result = 0;
    glGetProgramiv(program, GL_LINK_STATUS, &result);

    if (result == 0)
    {
        // Allocate a fairly large text buffer to hold the failed link output.
        const size_t ErrorTextBufferSize = 4096;
        std::string errorText(ErrorTextBufferSize, '\0');

        // Get the reason for link failure.
        glGetProgramInfoLog(program, ErrorTextBufferSize, nullptr, &errorText[0]);

        // Throw an exception telling the user that the shader program failed to link.
        throw DaybreakShaderLinkException(programName, errorText);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::VerifyShaderCompiled(unsigned int shader, const std::string& shaderName)
{
    int result = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &result);

    // Check that compilation succeeded. If it did not then get compilation output and throw an exception.
    if (result == 0)
    {
        // Allocate a fairly large text buffer to hold the failed compilation output.
        const size_t ErrorTextBufferSize = 4096;
        std::string errorText(ErrorTextBufferSize, '\0');

        // Get the reason for compilation failure.
        glGetShaderInfoLog(shader, ErrorTextBufferSize, nullptr, &errorText[0]);

        // Throw an exception telling the user that the shader failed to compile.
        throw DaybreakShaderCompileException(shaderName, errorText);
    }
}