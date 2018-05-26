#pragma once
#include "OglException.h"

namespace Daybreak::OpenGlRenderer
{
    //
    void glCheckForErrorsImpl(const char * filename, int line);
}


#ifdef _DEBUG
#define glCheckForErrors() glCheckForErrorsImpl(__FILE__, __LINE__);
#else
#define glCheckForErrors() 
#endif
