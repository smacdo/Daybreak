#include "stdafx.h"
#include "Texture.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::setWrap(TextureWrapMode s, TextureWrapMode t)
{
    setWrapS(s);
    setWrapT(t);
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::setWrap(TextureWrapMode s, TextureWrapMode t, TextureWrapMode u)
{
    setWrapS(s);
    setWrapT(t);
    setWrapU(u);
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::setWrapS(TextureWrapMode mode)
{
    m_wrapS = mode;
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::setWrapT(TextureWrapMode mode)
{
    m_wrapT = mode;
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::setWrapU(TextureWrapMode mode)
{
    m_wrapU = mode;
}
