#include "stdafx.h"
#include "OglRenderContext.h"
#include "OglError.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglRenderContext::OglRenderContext()
    : IRenderContext()
{
}

//---------------------------------------------------------------------------------------------------------------------
OglRenderContext::~OglRenderContext()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglRenderContext::destroy()
{
    // TODO: Not sure if needed.
}
