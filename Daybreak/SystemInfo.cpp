#include "stdafx.h"
#include "SystemInfo.h"

#include <glad\glad.h>
#include <cassert>

//---------------------------------------------------------------------------------------------------------------------
size_t SystemInfo::GlMaxVertexAttributeCount()
{
    assert(glGetIntegerv != nullptr && "GLAD must be sucessfully initialized to get OpenGL info");

    int attributeCount;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &attributeCount);

    return static_cast<size_t>(attributeCount);
}