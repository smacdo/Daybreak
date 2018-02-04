#include "stdafx.h"
#include "SceneRenderer.h"
#include "Scene.h"
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
    glUseProgram(m_standardShaderProgram);
    glBindVertexArray(m_standardVAO);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    // Make sure no errors happened while drawing.
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void SceneRenderer::CreateDefaultScene()
{
    // Create the standard VAO which defines Daybreak's standard vertex attribute layout.
    glGenVertexArrays(1, &m_standardVAO);
    glBindVertexArray(m_standardVAO);


    // Create a simple mesh to render.
    // TODO: Remove this once scene loading is added.
    const float DefaultVertices[] =
    {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    // Upload vertex data.
    unsigned int vbo = 0;
    glGenBuffers(1, &vbo);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glCheckForErrors();

    glBufferData(GL_ARRAY_BUFFER, sizeof(DefaultVertices), DefaultVertices, GL_STATIC_DRAW);
    glCheckForErrors();

    // Create standard vertex and pixel shaders.
    unsigned int standardVertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(standardVertexShader, 1, &StandardVertexShaderSource, nullptr);
    glCheckForErrors();

    glCompileShader(standardVertexShader);
    glCheckForErrors();

    VerifyShaderCompiled(standardVertexShader, "StandardVertexShader");

    unsigned int standardFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    
    glShaderSource(standardFragmentShader, 1, &StandardFragmentShaderSource, nullptr);
    glCheckForErrors();

    glCompileShader(standardFragmentShader);
    glCheckForErrors();

    VerifyShaderCompiled(standardFragmentShader, "StandardFragmentShader");

    // Create standard shader program.
    m_standardShaderProgram = glCreateProgram();

    glAttachShader(m_standardShaderProgram, standardVertexShader);
    glCheckForErrors();

    glAttachShader(m_standardShaderProgram, standardFragmentShader);
    glCheckForErrors();

    glLinkProgram(m_standardShaderProgram);
    glCheckForErrors();

    VerifyShaderLinked(m_standardShaderProgram, "StandardShader");

    // Activate newly created standard shader and destroy temporary shader pieces.
    glUseProgram(m_standardShaderProgram);
    glCheckForErrors();

    glDeleteShader(standardVertexShader);
    glCheckForErrors();

    glDeleteShader(standardFragmentShader);
    glCheckForErrors();

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