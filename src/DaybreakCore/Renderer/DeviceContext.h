#pragma once
#include <memory>

#include "Renderer\Texture.h"

namespace Daybreak
{
    class Image;

    /*** Hardware independent API for managing rendering resources. */
    class IDeviceContext
    {
    public:
        /** Initialize device context to default state. */
        IDeviceContext() = default;

        /** Copy constructor (deleted). */
        IDeviceContext(const IDeviceContext&) = delete;

        /** Destructor. */
        virtual ~IDeviceContext() = default;

        /** Assignment operator (deleted). */
        IDeviceContext& operator =(const IDeviceContext&) = delete;

    public:
        /** Create a new 2d texture. */
        virtual std::unique_ptr<ITexture2d> createTexture2d(
            const Image& image,                     ///< Source image.
            const TextureParameters& params) = 0;   ///< Texture sampling and generation parameters.
    };
}