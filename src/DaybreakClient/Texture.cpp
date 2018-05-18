#include "stdafx.h"
#include "Texture.h"
#include "Graphics/Image.h"
#include "Common/Error.h"

#include <glad\glad.h>

#include <memory>
#include <string>
#include <cassert>

namespace
{
    //-----------------------------------------------------------------------------------------------------------------
    unsigned int ToGl(TextureWrapMode mode)
    {
        switch (mode)
        {
        case TextureWrapMode::Repeat:
            return GL_REPEAT;
        case TextureWrapMode::Mirror:
            return GL_MIRRORED_REPEAT;
        case TextureWrapMode::ClampEdge:
            return GL_CLAMP_TO_EDGE;
        case TextureWrapMode::ClampBorder:
            return GL_CLAMP_TO_BORDER;
        default:
            throw InvalidEnumerationValueException("TextureWrapMode", static_cast<int>(mode));
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    unsigned int ToGl(ImagePixelFormat format)
    {
        switch (format)
        {
        case ImagePixelFormat::Grayscale:
            return GL_RED;
        case ImagePixelFormat::GrayscaleWithAlpha:
            return GL_RG;
        case ImagePixelFormat::RGB:
            return GL_RGB;
        case ImagePixelFormat::RGBA:
            return GL_RGBA;
        default:
            throw InvalidEnumerationValueException("ImagePixelFormat", static_cast<int>(format));
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    unsigned int ToGl(TextureFormat format)
    {
        switch (format)
        {
        case TextureFormat::Grayscale:
            return GL_RED;
        case TextureFormat::GrayscaleWithAlpha:
            return GL_RG;
        case TextureFormat::RGB:
            return GL_RGB;
        case TextureFormat::RGBA:
            return GL_RGBA;
        default:
            throw InvalidEnumerationValueException("TextureFormat", static_cast<int>(format));
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::SetWrap(TextureWrapMode s, TextureWrapMode t)
{
    SetWrapS(s);
    SetWrapT(t);
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::SetWrap(TextureWrapMode s, TextureWrapMode t, TextureWrapMode u)
{
    SetWrapS(s);
    SetWrapT(t);
    SetWrapU(u);
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::SetWrapS(TextureWrapMode mode)
{
    m_wrapS = mode;
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::SetWrapT(TextureWrapMode mode)
{
    m_wrapT = mode;
}

//---------------------------------------------------------------------------------------------------------------------
void TextureParameters::SetWrapU(TextureWrapMode mode)
{
    m_wrapU = mode;
}

//---------------------------------------------------------------------------------------------------------------------
Texture::Texture(unsigned int textureObject, const std::string& name)
    : m_texture(textureObject),
      m_name(name)
{
    // Ensure this object is actually a texture.
    if (!glIsTexture(textureObject))
    {
        throw ObjectNotTextureException(textureObject, name);
    }
}

//---------------------------------------------------------------------------------------------------------------------
Texture::~Texture()
{
    glDeleteTextures(1, &m_texture);
}

//---------------------------------------------------------------------------------------------------------------------
void Texture::Activate()
{   
    glBindTexture(GL_TEXTURE_2D, m_texture);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
void Texture::Activate(unsigned int textureUnit)
{
    // TODO: Test at runtime max number of slots and check that this slot is valid. Throw exception not assert.
    assert(textureUnit < 16 && "Must be a valid texture unit slot");

    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glCheckForErrors();

    glBindTexture(GL_TEXTURE_2D, m_texture);
    glCheckForErrors();
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<Texture> Texture::Create2d(
    const Image& image,
    TextureParameters settings,
    TextureFormat hardwareFormat)
{
    // TODO: Save current 2d texture id and restore it at the end.
    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glCheckForErrors();

    // Set texture wrapping parameters.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGl(settings.WrapS()));
    glCheckForErrors();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGl(settings.WrapT()));
    glCheckForErrors();

    // Set texture filter parameters. Note that min filter also takes into account mip map settings but mag filter
    // does not (because magnifciation cannot use mip maps).
    if (settings.MipFilter() == TextureFilterMode::Nearest)
    {
        if (settings.MinFilter() == TextureFilterMode::Nearest)     
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
            glCheckForErrors();
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            glCheckForErrors();
        }
    }
    else
    {
        if (settings.MinFilter() == TextureFilterMode::Nearest)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
            glCheckForErrors();
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glCheckForErrors();
        }
    }
    
    if (settings.MagFilter() == TextureFilterMode::Nearest)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glCheckForErrors();
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glCheckForErrors();
    }

    // Upload the image to the GPU.
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        ToGl(hardwareFormat),
        static_cast<GLsizei>(image.Width()),
        static_cast<GLsizei>(image.Height()),
        0,
        ToGl(image.Format()),
        GL_UNSIGNED_BYTE,
        image.RawPixels(0));
    glCheckForErrors();

    // Handle image mipmaps.
    //  TODO: Add option if mipmaps should be forced generated.
    //  TODO: Handle when image has mipmaps -> upload them.
    //  TODO: Handle when image does not have mipmaps -> generate them (unless disabled).
    glGenerateMipmap(GL_TEXTURE_2D);
    glCheckForErrors();

    // Return a newly created texture.
    return std::unique_ptr<Texture>(new Texture(texture, image.Name()));
}
