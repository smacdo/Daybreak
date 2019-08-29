#include "stdafx.h"
#include "OglDeviceContext.h"
#include "OglTexture.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglDeviceContext::OglDeviceContext()
    : IDeviceContext()
{
}

//---------------------------------------------------------------------------------------------------------------------
OglDeviceContext::~OglDeviceContext()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<ITexture2d> OglDeviceContext::createTexture2d(
    const Image& image,
    const TextureParameters& params)
{
    return OglTexture2d::generate(image, params);
}

//---------------------------------------------------------------------------------------------------------------------
void OglDeviceContext::destroy()
{
    // TODO: Not sure if needed.
}
