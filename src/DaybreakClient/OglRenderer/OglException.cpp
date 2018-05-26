#include "stdafx.h"
#include "OglException.h"

#include <string>
#include <exception>

#include <glad\glad.h>

using namespace Daybreak::OpenGlRenderer;

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
