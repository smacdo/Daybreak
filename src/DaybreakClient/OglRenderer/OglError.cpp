#include "stdafx.h"
#include "OglError.h"

#include <string>
#include <exception>

#include <glad\glad.h>

using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
void Daybreak::OpenGlRenderer::glCheckForErrorsImpl(const char * filename, int line)
{
    GLenum errorCode = GL_NO_ERROR;

    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        throw GlException(errorCode, filename, line);
    }
}
