#include "stdafx.h"
#include "Error.h"

#include <string>
#include <exception>

#include <SDL.h>
#include <glad\glad.h>

//---------------------------------------------------------------------------------------------------------------------
void glCheckForErrorsImpl(const char * filename, int line)
{
    GLenum errorCode = GL_NO_ERROR;

    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        throw GlException(errorCode, filename, line);
    }
}

//---------------------------------------------------------------------------------------------------------------------
void DisplayErrorDialog(
    const std::string& title,
    const std::string& message)
{
    // TODO: Improve.
    MessageBoxA(nullptr, message.c_str(), title.c_str(), 0);
}


//---------------------------------------------------------------------------------------------------------------------
SdlException::SdlException(const std::string& message)
    : std::runtime_error(message + std::string(": ") + GetLastSdlError()),
      m_message(message)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string SdlException::GetLastSdlError()
{
    return SDL_GetError();
}

//---------------------------------------------------------------------------------------------------------------------
GladException::GladException(const std::string& message)
    : std::runtime_error(message)
{
}

//---------------------------------------------------------------------------------------------------------------------
GlException::GlException(unsigned int error, const char * filename, int line)
    : std::runtime_error(
        std::string("OpenGL error " + std::to_string(error) + " (" + GetErrorName(error) + ") was caught")),
      m_error(error),
      m_filename(filename),
      m_line(line)
{
}

//---------------------------------------------------------------------------------------------------------------------
const char * GlException::ErrorName() const
{
    return GetErrorName(m_error);
}

//---------------------------------------------------------------------------------------------------------------------
const char * GlException::GetErrorName(unsigned int error)
{
    switch (error)
    {
    case GL_INVALID_ENUM:
        return "GL_INVALID_ENUM";
    case GL_INVALID_VALUE:
        return "GL_INVALID_VALUE";
    case GL_INVALID_OPERATION:
        return "GL_INVALID_OPERATION";
    case GL_INVALID_INDEX:
        return "GL_INVALID_INDEX";
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        return "GL_INVALID_FRAMEBUFFER_OPERATION";
    case GL_OUT_OF_MEMORY:
        return "GL_OUT_OF_MEMORY";
    case GL_NO_ERROR:
        return "GL_NO_ERROR";
    default:
        return "UNKNOWN";
    }
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakEngineException::DaybreakEngineException(const std::string& message, const std::string& details)
    : std::runtime_error(message),
      m_message(message),
      m_details(details)
{
}

//---------------------------------------------------------------------------------------------------------------------
RuntimeCheckException::RuntimeCheckException(
    const char * message,
    const char * expression,
    const char * file,
    unsigned int lineNumber)
    : DaybreakEngineException(
        message == nullptr ? "A runtime check has failed" : message,
        buildDetailsString(expression, file, lineNumber)),
      m_expression(expression),
      m_file(file),
      m_lineNumber(lineNumber)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string RuntimeCheckException::buildDetailsString(
    const char * expression,
    const char * file,
    unsigned int lineNumber)
{
    return std::string(expression) + "\n(" + file + ":" + std::to_string(lineNumber) + ")";
}

//---------------------------------------------------------------------------------------------------------------------
InvalidEnumerationValueException::InvalidEnumerationValueException(const std::string& enumTypeName, int value)
    : DaybreakEngineException(
        "Invalid or unexpected enumeration value",
        "Enumeration type " + enumTypeName + ", value was " + std::to_string(value)),
      m_enumTypeName(enumTypeName),
      m_value(value)
{
}

//---------------------------------------------------------------------------------------------------------------------
ObjectNotShaderException::ObjectNotShaderException(unsigned int objectId, const std::string& shaderName)
    : DaybreakEngineException("Expected object id " + std::to_string(objectId) + " to be a shader", shaderName),
      m_shaderName(shaderName),
      m_objectId(objectId)
{
}

//---------------------------------------------------------------------------------------------------------------------
ObjectNotTextureException::ObjectNotTextureException(unsigned int objectId, const std::string& textureName)
    : DaybreakEngineException("Expected object id " + std::to_string(objectId) + " to be a texture", textureName),
      m_textureName(textureName),
      m_objectId(objectId)
{
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakShaderCompileException::DaybreakShaderCompileException(
    const std::string& shaderName,
    const std::string& compileInfo)
    : DaybreakEngineException("Failed to compile shader " + shaderName, compileInfo),
      m_shaderName(shaderName),
      m_compileInfo(compileInfo)
{
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakShaderLinkException::DaybreakShaderLinkException(
    const std::string& shaderName,
    const std::string& linkInfo)
    : DaybreakEngineException("Failed to link shader " + shaderName, linkInfo),
      m_shaderName(shaderName),
      m_linkInfo(linkInfo)
{
}

//---------------------------------------------------------------------------------------------------------------------
ContentReadException::ContentReadException(
    const std::string& contentFilePath,
    const std::string& contentTypeName,
    const std::string& errorMessage)
    : DaybreakEngineException(
        errorMessage,
        "Failed to load content type " + contentTypeName + " from file " + contentFilePath),
      m_filePath(contentFilePath),
      m_typeName(contentTypeName)
{
}
