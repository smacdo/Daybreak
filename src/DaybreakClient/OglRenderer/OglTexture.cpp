#include "stdafx.h"
#include "OglTexture.h"
#include "OglError.h"
#include "Content\Images\Image.h"

#include <glad\glad.h>

#include <memory>
#include <string>
#include <cassert>

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

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
            THROW_ENUM_SWITCH_NOT_HANDLED(TextureWrapMode, mode);
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
            THROW_ENUM_SWITCH_NOT_HANDLED(ImagePixelFormat, format);
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
            THROW_ENUM_SWITCH_NOT_HANDLED(TextureFormat, format);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglTexture2d::OglTexture2d(GLuint textureId, const std::string& name)
    : ITexture2d(),
      m_name(name)
{
    if (textureId != 0)
    {
        setTextureId(textureId);
    }
}

//---------------------------------------------------------------------------------------------------------------------
OglTexture2d::~OglTexture2d()
{
    destroy();
}

//---------------------------------------------------------------------------------------------------------------------
void OglTexture2d::destroy()
{
    if (glIsTexture(m_id) == GL_TRUE)
    {
        GLuint textures[] = { m_id };
        glDeleteBuffers(1, std::begin(textures));

        m_id = 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglTexture2d::setTextureId(GLuint id)
{
    if (glIsTexture(id) == GL_FALSE)
    {
        throw ObjectNotTextureException(id, m_name);
    }

    m_id = id;
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglTexture2d> OglTexture2d::generate(
    const Image& image,
    TextureParameters settings)
{
    return std::move(generate(image, settings, settings.format()));
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<OglTexture2d> OglTexture2d::generate(
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
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ToGl(settings.wrapS()));
    glCheckForErrors();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ToGl(settings.wrapT()));
    glCheckForErrors();

    // Set texture filter parameters. Note that min filter also takes into account mip map settings but mag filter
    // does not (because magnifciation cannot use mip maps).
    if (settings.mipFilter() == TextureFilterMode::Nearest)
    {
        if (settings.minFilter() == TextureFilterMode::Nearest)
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
        if (settings.minFilter() == TextureFilterMode::Nearest)
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

    if (settings.magFilter() == TextureFilterMode::Nearest)
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
    return std::unique_ptr<OglTexture2d>(new OglTexture2d(texture, image.Name()));
}