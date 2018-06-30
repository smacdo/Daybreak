#include "stdafx.h"
#include "OglShader.h"
#include "OglError.h"

#include <glad\glad.h>
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
std::unique_ptr<OglShader> OglShader::generateFromFile(
    const std::string& shaderProgramName,
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath)
{
    // Load the vertex shader and fragment shaders from disk.
    auto vertexShaderString = ReadTextFromFile(vertexShaderPath);
    auto fragmentShaderString = ReadTextFromFile(fragmentShaderPath);
    
    // Use LoadFromString variant to finish loading the shader.
    return generate(
        shaderProgramName,
        vertexShaderPath,
        vertexShaderString,
        fragmentShaderPath,
        fragmentShaderString);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglShader> OglShader::generate(
    const std::string& shaderProgramName,
    const std::string& vertexShaderName,
    const std::string& vertexShaderString,
    const std::string& fragmentShaderName,
    const std::string& fragmentShaderString)
{
    auto vertexShaderText = vertexShaderString.c_str();
    auto fragmentShaderText = fragmentShaderString.c_str();

    // Create and upload vertex shader.
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);

    glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
    glCheckForErrors();

    glCompileShader(vertexShader);
    glCheckForErrors();

    VerifyShaderCompiled(vertexShader, vertexShaderName);

    // Create and upload fragment shader.
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
    glCheckForErrors();

    glCompileShader(fragmentShader);
    glCheckForErrors();

    VerifyShaderCompiled(fragmentShader, fragmentShaderName);

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
std::string OglShader::ReadTextFromFile(const std::string& filepath)
{
    std::ifstream stream;

    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Enable exceptions on stream.
    stream.open(filepath);

    return std::string(std::istreambuf_iterator<char>(stream), {});
}

//---------------------------------------------------------------------------------------------------------------------
void OglShader::VerifyShaderLinked(GLuint program, const std::string& programName)
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
void OglShader::VerifyShaderCompiled(GLuint shader, const std::string& shaderName)
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