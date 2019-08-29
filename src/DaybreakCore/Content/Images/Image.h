#pragma once

#include <string>
#include <memory>

namespace StbSupport
{
    // Functor to delete data allocated by stb_image.
    struct ImageDeleteFunctor
    {
        void operator()(unsigned char * pData) const;
        void operator()(float * pData) const;
    };
}

namespace Daybreak
{
    // Image pixel format.
    enum class ImagePixelFormat
    {
        None,                       // 0 byte.
        Grayscale,                  // 1 byte.
        GrayscaleWithAlpha,         // 2 byte.
        RGB,                        // 3 byte.
        RGBA                        // 4 byte.
    };

    /** Base class for image data. */
    class Image
    {
    protected:
        /** Constructor. */
        Image(
            const std::string& name,
            size_t imageWidth,
            size_t imageHeight,
            ImagePixelFormat pixelFormat);

    public:
        /** Destructor. */
        virtual ~Image();

        /** Get name of image. (Optional, possibly null). */
        std::string Name() const noexcept { return m_name; }

        /** Check if the image is valid. A valid image has pixel data and non-zero attributes. */
        virtual bool IsValid() const noexcept = 0;

        /** Get the number of mipmaps contained in this image. A value one is just the image at normal size. */
        size_t MipMapCount() const noexcept { return 1; }

        /** Get width of image in pixels. This returns zero if there is no valid image data. */
        size_t Width() const noexcept { return m_width; }

        /** Get height of image in pixels. This returns zero if there is no valid image data. */
        size_t Height() const noexcept { return m_height; }

        /** Get pixel format of image. This returns ImagePixelFormat::None if there is no valid image data. */
        ImagePixelFormat Format() const noexcept { return m_format; }

        /** Get read-only non-owned pointer to the raw pixel data. */
        virtual const unsigned char* RawPixels(unsigned int mipMapIndex = 0) const noexcept = 0;

    public:
        std::string m_name;
        size_t m_width;
        size_t m_height;
        ImagePixelFormat m_format;
    };

    /**
     * Represents a traditional multi-channel RGB image where each channel is a single byte integer value. This
     * includes almost all traditional, non-HDR formatted pictures including BMP, PNG, JPG, TGA, etc.
     */
    class BytePerChannelImage : public Image
    {
    protected:
        /** Constructor. */
        BytePerChannelImage(
            const std::string& name,
            size_t imageWidth,
            size_t imageHeight,
            ImagePixelFormat pixelFormat,
            std::unique_ptr<unsigned char[], StbSupport::ImageDeleteFunctor> pixels);

    public:
        /** Destructor. */
        ~BytePerChannelImage();

        /** Check if the image is valid. A valid image has pixel data and non-zero attributes. */
        virtual bool IsValid() const noexcept override { return m_pixels != nullptr; }
         
        /** Get read-only non-owned pointer to the raw pixel data. */
        virtual const unsigned char* RawPixels(unsigned int mipMapIndex = 0) const noexcept override
        {
            return m_pixels.get();
        }

        // Load an image from file.
        // TODO: Do not do file loading here, instead take an IBuffer or IFileStream and let loading be handled by another
        //       system.
        static std::unique_ptr<BytePerChannelImage> LoadFromFile(const std::string& imageFilePath);

    private:
        std::unique_ptr<unsigned char[], StbSupport::ImageDeleteFunctor> m_pixels;
    };

    /**
     * Holds the data for an HDR image which is an RGB image with each channel being represnted by a float rather than
     * a byte.
     */
    class HdrImage : public Image
    {
    protected:
        /** Constructor. */
        HdrImage(
            const std::string& name,
            size_t imageWidth,
            size_t imageHeight,
            ImagePixelFormat pixelFormat,
            std::unique_ptr<float[], StbSupport::ImageDeleteFunctor> pixels);

    public:
        /** Destructor. */
        ~HdrImage();

        /** Check if the image is valid. A valid image has pixel data and non-zero attributes. */
        virtual bool IsValid() const noexcept override { return m_pixels != nullptr; }

        /** Get read-only non-owned pointer to the raw pixel data. */
        virtual const unsigned char* RawPixels(unsigned int mipMapIndex = 0) const noexcept override
        {
            return reinterpret_cast<const unsigned char *>(m_pixels.get());
        }

        /** Get read-only non-owned pointer to the raw pixel data. */
        const float * RawPixelsAsFloatBuffer(unsigned int mipMapIndex = 0) const noexcept
        {
            return m_pixels.get();
        }

        // Load an image from file.
        // TODO: Do not do file loading here, instead take an IBuffer or IFileStream and let loading be handled by another
        //       system.
        static std::unique_ptr<HdrImage> LoadFromFile(const std::string& imageFilePath);

    private:
        std::unique_ptr<float[], StbSupport::ImageDeleteFunctor> m_pixels;
    };
}
