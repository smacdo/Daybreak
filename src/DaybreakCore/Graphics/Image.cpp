#include "stdafx.h"
#include "Image.h"
#include "Common/Error.h"

#define STBI_NO_STDIO
#include "stb\stb_image.h"

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <cassert>

#include <mutex>

using namespace Daybreak;

namespace
{
    // stb_image error reporting is not thread safe so a global image lock is used to guard stb's image loading and
    // error reporting code path.
    std::mutex GImageLoadingMutex;

    //-----------------------------------------------------------------------------------------------------------------
    ImagePixelFormat FromStbChannelCount(int channelCount)
    {
        switch (channelCount)
        {
        case 1:
            return ImagePixelFormat::Grayscale;
        case 2:
            return ImagePixelFormat::GrayscaleWithAlpha;
        case 3:
            return ImagePixelFormat::RGB;
        case 4:
            return ImagePixelFormat::RGBA;
        default:
            THROW_ENUM_SWITCH_NOT_HANDLED(ImagePixelFormat, channelCount);
        }
    }

    //-----------------------------------------------------------------------------------------------------------------
    std::unique_ptr<unsigned char[]> ReadBytesFromFile(const std::string& filePath, size_t * pFileSizeOut)
    {
        // Open binary file stream to given image and get the file size.
        std::ifstream stream;

        stream.exceptions(std::ifstream::failbit | std::ifstream::badbit);  // Enable exceptions on stream.
        stream.open(filePath, std::ios::binary);

        stream.seekg(0, std::ios::end);
        auto size = stream.tellg();

        stream.seekg(0, std::ios::beg);

        // Read bytes from the stream.
        std::unique_ptr<unsigned char[]> fileBytes(new unsigned char[size]);

        stream.read(reinterpret_cast<char *>(fileBytes.get()), size);       // TODO: Check cast.
        stream.close();

        // Write the file size if the caller passed a valid pointer.
        if (pFileSizeOut != nullptr)
        {
            *pFileSizeOut = size;
        }

        // Return bytes read.
        return fileBytes;
    }
}

//---------------------------------------------------------------------------------------------------------------------
Image::Image(
    const std::string& name,
    size_t imageWidth,
    size_t imageHeight,
    ImagePixelFormat pixelFormat,
    std::unique_ptr<unsigned char[], StbSupport::ImageDeleteFunctor> pixels)
    : m_name(name),
      m_width(imageWidth),
      m_height(imageHeight),
      m_format(pixelFormat),
      m_pixels(std::move(pixels))
{
    // Check that any image with pixel data has non-zero attributes.
    bool validPixels = (m_pixels != nullptr);
    bool nonZeroAttributes = (m_width != 0 && m_height != 0);
    bool validFormat = (m_format > ImagePixelFormat::None && m_format <= ImagePixelFormat::RGBA);
    assert((validPixels == nonZeroAttributes == validFormat) && "Only empty images can have zero value properties");
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<Image> Image::LoadFromFile(const std::string& imageFilePath)
{
    // TODO: Do not do file loading here, instead take an IBuffer or IFileStream and let loading be handled by another
    //       system.

    // Read the file into memory.
    size_t size = 0u;
    auto fileBytes = ReadBytesFromFile(imageFilePath, &size);

    // Use stb_image to load the image into memory. Note that stb_image's error reporting is not thread safe so access
    // to image loading must be guarded. (Sadly).
    int imageWidth = 0, imageHeight = 0, channelCount = 0;
    std::unique_ptr<unsigned char[], StbSupport::ImageDeleteFunctor> rawPixels;
    
    {
        std::lock_guard<std::mutex> lock(GImageLoadingMutex);

        // TODO: Make this a flag rather than always flip it for OpenGL.
        stbi_set_flip_vertically_on_load(true);

        // Load the image into a raw pixel buffer.
        rawPixels.reset(stbi_load_from_memory(
            fileBytes.get(),
            static_cast<int>(size),
            &imageWidth,
            &imageHeight,
            &channelCount,
            0));

        // Check for errors while loading. If there were any errors throw an exception rather than returning a null
        // image.
        if (rawPixels == nullptr)
        {
            throw ContentReadException(imageFilePath, "Image", stbi_failure_reason());
        }
    }

    assert(rawPixels != nullptr && "An exception should have be thrown if the image was not loaded");

    // Create and return new image.
    auto format = FromStbChannelCount(channelCount);
    return std::unique_ptr<Image>(new Image(imageFilePath, imageWidth, imageHeight, format, std::move(rawPixels)));
}

//---------------------------------------------------------------------------------------------------------------------
HdrImage::HdrImage(
    const std::string& name,
    size_t imageWidth,
    size_t imageHeight,
    ImagePixelFormat pixelFormat,
    std::unique_ptr<float[], StbSupport::ImageDeleteFunctor> pixels)
    : m_name(name),
      m_width(imageWidth),
      m_height(imageHeight),
      m_format(pixelFormat),
      m_pixels(std::move(pixels))
{
    // Check that any image with pixel data has non-zero attributes.
    bool validPixels = (pixels != nullptr);
    bool nonZeroAttributes = (m_width != 0 && m_height != 0);
    bool validFormat = (m_format > ImagePixelFormat::None && m_format <= ImagePixelFormat::RGBA);
    assert((validPixels == nonZeroAttributes == validFormat) && "Only empty images can have zero value properties");
}

//---------------------------------------------------------------------------------------------------------------------
std::unique_ptr<HdrImage> HdrImage::LoadFromFile(const std::string& imageFilePath)
{
    // TODO: Do not do file loading here, instead take an IBuffer or IFileStream and let loading be handled by another
    //       system.

    // Read the file into memory.
    size_t size = 0u;
    auto fileBytes = ReadBytesFromFile(imageFilePath, &size);

    // Use stb_image to load the image into memory. Note that stb_image's error reporting is not thread safe so access
    // to image loading must be guarded. (Sadly).
    int imageWidth = 0, imageHeight = 0, channelCount = 0;
    std::unique_ptr<float[], StbSupport::ImageDeleteFunctor> rawPixels;

    {
        std::lock_guard<std::mutex> lock(GImageLoadingMutex);

        // TODO: Make this a flag rather than always flip it for OpenGL.
        stbi_set_flip_vertically_on_load(true);

        // Load the image into a raw pixel buffer.
        rawPixels.reset(stbi_loadf_from_memory(
            fileBytes.get(),
            static_cast<int>(size),
            &imageWidth,
            &imageHeight,
            &channelCount,
            0));

        // Check for errors while loading. If there were any errors throw an exception rather than returning a null
        // image.
        if (rawPixels == nullptr)
        {
            throw ContentReadException(imageFilePath, "Image", stbi_failure_reason());
        }
    }

    assert(rawPixels != nullptr && "An exception should have be thrown if the image was not loaded");

    // Create and return new image.
    auto format = FromStbChannelCount(channelCount);
    return std::unique_ptr<HdrImage>(
        new HdrImage(imageFilePath, imageWidth, imageHeight, format, std::move(rawPixels)));
}

//---------------------------------------------------------------------------------------------------------------------
void StbSupport::ImageDeleteFunctor::operator()(unsigned char * pData) const
{
    stbi_image_free(pData);
}

void StbSupport::ImageDeleteFunctor::operator()(float * pData) const
{
    stbi_image_free(pData);
}
