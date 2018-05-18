#pragma once

#include <memory>
#include <string>

class Image;

/** Specifies how textures are treated outside of the [0, 1] UV range. */
enum class TextureWrapMode
{
    Repeat,
    Mirror,
    ClampEdge,
    ClampBorder
};

/** Specifies how texture and mipmap filter should be done. */
enum class TextureFilterMode
{
    Nearest,
    Linear
};

/** Specify texture format. */
enum class TextureFormat
{
    Grayscale,
    GrayscaleWithAlpha,
    RGB,
    RGBA
};

/** Defines how a texture should be sampled by the GPU. */
class TextureParameters
{
public:
    /** Get minification filter mode. */
    TextureFilterMode MinFilter() const { return m_minFilter; }

    /** Set minification filter mode. */
    void SetMinFilter(TextureFilterMode mode) { m_minFilter = mode; }

    /** Get magnification filter mode. */
    TextureFilterMode MagFilter() const { return m_magFilter; }

    /** Set magnification filter mode. */
    void SetMagFilter(TextureFilterMode mode) { m_magFilter = mode; }

    /** Get mipmap filter mode. */
    TextureFilterMode MipFilter() const { return m_mipFilter; }

    /** Set mipmap filter mode. */
    void SetMipFilter(TextureFilterMode mode) { m_mipFilter = mode; }

    /** Set texture wrapping mode for 2d texture paramters s and t. */
    void SetWrap(TextureWrapMode s, TextureWrapMode t);

    /** Set texture wrapping mode for 3d texture parameters s, t and u. */
    void SetWrap(TextureWrapMode s, TextureWrapMode t, TextureWrapMode u);

    /** Get texture wrapping mode for parameter s. */
    TextureWrapMode WrapS() const { return m_wrapS; }

    /** Set texture wrapping mode for parameter s. */
    void SetWrapS(TextureWrapMode mode);

    /** Get texture wrapping mode for parameter T. */
    TextureWrapMode WrapT() const { return m_wrapT; }

    /** Set texture wrapping mode for parameter u. */
    void SetWrapT(TextureWrapMode mode);

    /** Get texture wrapping mode for parameter u. */
    TextureWrapMode WrapU() const { return m_wrapU; }

    /** Set texture wrapping mode for parameter v. */
    void SetWrapU(TextureWrapMode mode);

private:
    TextureWrapMode m_wrapS = TextureWrapMode::Repeat;
    TextureWrapMode m_wrapT = TextureWrapMode::Repeat;
    TextureWrapMode m_wrapU = TextureWrapMode::Repeat;
    TextureFilterMode m_minFilter = TextureFilterMode::Nearest;
    TextureFilterMode m_magFilter = TextureFilterMode::Nearest;
    TextureFilterMode m_mipFilter = TextureFilterMode::Linear;
};

/** Defines a texture object stored on the GPU. */
// TODO: Make non-copyable.
class Texture
{
protected:
    /** Constructor. */
    Texture(unsigned int textureObject, const std::string& name);

public:
    /** Destructor. */
    ~Texture();

    /** Make texture active. */
    void Activate();

    /** Make texture active in a specific slot. */
    void Activate(unsigned int textureUnit);

    /** Get texture name. (Optional, might be empty.) */
    std::string Name() const { return m_name; }

    /** Create 2d texture. */
    static std::unique_ptr<Texture> Create2d(
        const Image& image,
        TextureParameters settings,
        TextureFormat hardwareFormat);

private:
    unsigned int m_texture;
    std::string m_name;
};

