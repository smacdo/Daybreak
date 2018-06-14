#pragma once
#include "Renderer/RenderContext.h"
#include <glad\glad.h>
#include <memory>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL index buffer encapsulation.
    class OglRenderContext final : public IRenderContext
    {
    public:
        // Constructor.
        OglRenderContext();

        // Destructor.
        ~OglRenderContext();

    private:
        // Releases allocated resources (not sure if needed).
        void destroy();
    };
}