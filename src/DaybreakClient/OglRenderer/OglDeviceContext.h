#pragma once
#include "Renderer\DeviceContext.h"
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    /** OpenGL implementation of DeviceContext. */
    class OglDeviceContext final : public IDeviceContext
    {
    public:
        /** Initialize device context to default state. */
        OglDeviceContext();

        /** Destructor. */
        ~OglDeviceContext();

    public:
        /** Create a new 2d texture. */
        virtual std::unique_ptr<ITexture2d> createTexture2d(
            const Image& image,                         ///< Source image.
            const TextureParameters& params) override;  ///< Texture sampling and generation parameters.

    protected:
        /** Destroy allocated resources. */
        void destroy();
    };
}