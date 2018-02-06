#include "stdafx.h"
#include "Shader.h"
#include "ErrorHandling.h"

#include <glad\glad.h>
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <string>
#include <fstream>

//---------------------------------------------------------------------------------------------------------------------
Shader::Shader(const std::string& shaderName, unsigned int shaderProgram)
    : m_name(shaderName),
      m_shaderProgram(shaderProgram)
{
    // Verify that shader is valid.
    if (!glIsProgram(shaderProgram))
    {
        throw ObjectNotShaderException(shaderName, shaderProgram);
    }
}

//---------------------------------------------------------------------------------------------------------------------
Shader::~Shader()
{
    glDeleteProgram(m_shaderProgram);
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::Activate()
{
    glUseProgram(m_shaderProgram);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_shaderProgram, name.c_str()), value);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(m_shaderProgram, name.c_str()), static_cast<int>(value));
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector2f(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_shaderProgram, name.c_str()), x, y);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector2f(const std::string& name, const glm::vec2& v) const
{
    glUniform2f(glGetUniformLocation(m_shaderProgram, name.c_str()), v.x, v.y);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector3f(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_shaderProgram, name.c_str()), x, y, z);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector3f(const std::string& name, const glm::vec3& v) const
{
    glUniform3f(glGetUniformLocation(m_shaderProgram, name.c_str()), v.x, v.y, v.z);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector4f(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(m_shaderProgram, name.c_str()), x, y, z, w);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetVector4f(const std::string& name, const glm::vec4& v) const
{
    glUniform4f(glGetUniformLocation(m_shaderProgram, name.c_str()), v.x, v.y, v.z, v.w);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::SetMatrix4(const std::string& name, const glm::mat4& v) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram, name.c_str()), 1, GL_FALSE, glm::value_ptr(v));
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<Shader> Shader::LoadFromFile(
    const std::string& shaderProgramName,
    const std::string& vertexShaderPath,
    const std::string& fragmentShaderPath)
{
    // Load the vertex shader and fragment shaders from disk.
    auto vertexShaderString = ReadTextFromFile(vertexShaderPath);
    auto fragmentShaderString = ReadTextFromFile(fragmentShaderPath);
    
    // Use LoadFromString variant to finish loading the shader.
    return LoadFromString(
        shaderProgramName,
        vertexShaderPath,
        vertexShaderString,
        fragmentShaderPath,
        fragmentShaderString);
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<Shader> Shader::LoadFromString(
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
    return std::unique_ptr<Shader>(new Shader(shaderProgramName, shaderProgram));
}

//---------------------------------------------------------------------------------------------------------------------
std::string Shader::ReadTextFromFile(const std::string& filepath)
{
    std::ifstream stream;

    stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Enable exceptions on stream.
    stream.open(filepath);

    return std::string(std::istreambuf_iterator<char>(stream), {});
}

//---------------------------------------------------------------------------------------------------------------------
void Shader::VerifyShaderLinked(unsigned int program, const std::string& programName)
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
void Shader::VerifyShaderCompiled(unsigned int shader, const std::string& shaderName)
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