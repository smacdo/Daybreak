#include "stdafx.h"
#include "OglShader.h"
#include "OglError.h"
#include "Renderer\RendererExceptions.h"

#include <glad\glad.h>
#include <array>
#include <string>
#include <fstream>

using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglShader::OglShader(GLuint shaderProgram, const std::string& shaderName)
    : IShader(),
      m_name(shaderName)
{
    if (shaderProgram != 0)
    {
        setShaderProgram(shaderProgram);
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglShader::~OglShader()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglShader::destroy()
{
    if (glIsProgram(m_shaderProgram) == GL_TRUE)
    {
        GLuint shaders[] = { m_shaderProgram };
        glDeleteBuffers(1, std::begin(shaders));

        m_shaderProgram = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglShader::setShaderProgram(GLuint id)
{
    if (glIsProgram(id) == GL_FALSE)
    {
        throw ObjectNotShaderException(id, m_name);
    }

    m_shaderProgram = id;
}

//---------------------------------------------------------------------------------------------------------------------
Daybreak::ShaderVariable OglShader::getVariable(const std::string& name) const
{
    CHECK_NOT_EMPTY(name);

    auto address = glGetUniformLocation(m_shaderProgram, name.c_str());
    glCheckForErrors();

    if (address < 0)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Shader variable name is not valid");
    }

    return ShaderVariable(address);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglShader> OglShader::generate(
    const std::string& shaderProgramName,
    const std::string_view& vertexShaderString,
    const std::string_view& fragmentShaderString)
{
    std::array<const char *, 1> vertexShaderTexts{ vertexShaderString.data() };
    std::array<GLint, 1> vertexShaderLengths{ static_cast<GLint>(vertexShaderString.size()) };

    std::array<const char *, 1> fragmentShaderTexts{ fragmentShaderString.data() };
    std::array<GLint, 1> fragmentShaderLengths{ static_cast<GLint>(fragmentShaderString.size()) };

    // Create and upload vertex shader.
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderTexts[0], &vertexShaderLengths[0]);
    glCheckForErrors();

    glCompileShader(vertexShader);
    glCheckForErrors();

    VerifyShaderCompiled(vertexShader, shaderProgramName, "VertexShader");

    // Create and upload fragment shader.
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderTexts[0], &fragmentShaderLengths[0]);
    glCheckForErrors();

    glCompileShader(fragmentShader);
    glCheckForErrors();

    VerifyShaderCompiled(fragmentShader, shaderProgramName, "PixelShader");

    // Create standard shader program.
    auto shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glCheckForErrors();

    glAttachShader(shaderProgram, fragmentShader);
    glCheckForErrors();

    glLinkProgram(shaderProgram);
    glCheckForErrors();

    VerifyShaderLinked(shaderProgram, shaderProgramName);

    // Delete the vertex and fragment shaders because they have been linked into the shader program.
    glDeleteShader(vertexShader);
    glCheckForErrors();

    glDeleteShader(fragmentShader);
    glCheckForErrors();

    // Construct and return shader.
    return std::unique_ptr<OglShader>(new OglShader(shaderProgram, shaderProgramName));
}

//---------------------------------------------------------------------------------------------------------------------
void OglShader::VerifyShaderLinked(
    GLuint program,
    const std::string& programName)
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
void OglShader::VerifyShaderCompiled(
    GLuint shader,
    const std::string& programName,
    const std::string& stageName)
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
        throw DaybreakShaderCompileException(programName + "_" + stageName, errorText);
    }
}