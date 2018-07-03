#pragma once
#include "Renderer/Texture.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak
{
    class Image;
}

namespace Daybreak::OpenGlRenderer
{
    // OpenGL 2d texture.
    class OglTexture2d final : public ITexture2d
    {
    public:
        // Constructor.
        OglTexture2d(GLuint textureObject, const std::string& name);

        // Destructor.
        virtual ~OglTexture2d();

        // Get texture name. (Optional, might be empty.)
        virtual std::string name() const override { return m_name; }

        // Get texture buffer object.
        GLuint textureId() const noexcept { return m_id; }

        // Set texture id.
        void setTextureId(GLuint id);

        // Create 2d texture from an image.
        static std::unique_ptr<OglTexture2d> generate(
            const Image& image,
            TextureParameters settings,
            TextureFormat hardwareFormat);

    private:
        void destroy();

    private:
        GLuint m_id = 0;
        std::string m_name;
    };
}