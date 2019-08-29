#pragma once
#include "GpuResource.h"
#include <string>

namespace Daybreak
{
    // Specifies how textures are treated outside of the [0, 1] UV range.
    enum class TextureWrapMode
    {
        Repeat,
        Mirror,
        ClampEdge,
        ClampBorder
    };

    // Specifies how texture and mipmap filter should be done.
    enum class TextureFilterMode
    {
        Nearest,
        Linear
    };

    // Specify texture format.
    enum class TextureFormat
    {
        Grayscale,
        GrayscaleWithAlpha,
        RGB,
        RGBA
    };

    // Defines how a texture should be sampled by the GPU.
    class TextureParameters
    {
    public:
        // Get minification filter mode.
        TextureFilterMode minFilter() const { return m_minFilter; }

        // Set minification filter mode.
        void setMinFilter(TextureFilterMode mode) { m_minFilter = mode; }

        // Get magnification filter mode.
        TextureFilterMode magFilter() const { return m_magFilter; }

        // Set magnification filter mode.
        void setMagFilter(TextureFilterMode mode) { m_magFilter = mode; }

        // Get mipmap filter mode.
        TextureFilterMode mipFilter() const { return m_mipFilter; }

        // Set mipmap filter mode.
        void setMipFilter(TextureFilterMode mode) { m_mipFilter = mode; }

        // Set texture wrapping mode for 2d texture paramters s and t.
        void setWrap(TextureWrapMode s, TextureWrapMode t);

        // Set texture wrapping mode for 3d texture parameters s, t and u.
        void setWrap(TextureWrapMode s, TextureWrapMode t, TextureWrapMode u);

        // Get texture wrapping mode for parameter s.
        TextureWrapMode wrapS() const { return m_wrapS; }

        // Set texture wrapping mode for parameter s.
        void setWrapS(TextureWrapMode mode);

        // Get texture wrapping mode for parameter T.
        TextureWrapMode wrapT() const { return m_wrapT; }

        // Set texture wrapping mode for parameter u.
        void setWrapT(TextureWrapMode mode);

        // Get texture wrapping mode for parameter u.
        TextureWrapMode wrapU() const { return m_wrapU; }

        // Set texture wrapping mode for parameter v.
        void setWrapU(TextureWrapMode mode);

        // Get texture format.
        TextureFormat format() const { return m_format; }

        // Set texture format.
        void setFormat(TextureFormat format) { m_format = format; }

    private:
        TextureWrapMode m_wrapS = TextureWrapMode::Repeat;
        TextureWrapMode m_wrapT = TextureWrapMode::Repeat;
        TextureWrapMode m_wrapU = TextureWrapMode::Repeat;
        TextureFilterMode m_minFilter = TextureFilterMode::Nearest;
        TextureFilterMode m_magFilter = TextureFilterMode::Nearest;
        TextureFilterMode m_mipFilter = TextureFilterMode::Linear;
        TextureFormat m_format = TextureFormat::RGB;
    };

    // Render independent 2d texture interface.
    class ITexture2d : public GpuResource
    {
    public:
        // Destructor.
        virtual ~ITexture2d() = default;

        // Get texture name. (Optional, might be empty.)
        virtual std::string name() const = 0;
    };
}