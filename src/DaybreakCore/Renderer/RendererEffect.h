#pragma once

namespace Daybreak
{
    class IRenderContext;

    // Interface for a renderer effect to apply when drawing an object.
    class IRendererEffect
    {
    public:
        // Constructor.
        IRendererEffect() = default;

        // Copy constructor (deleted).
        IRendererEffect(const IRendererEffect&) = delete;

        // Destructor.
        virtual ~IRendererEffect() = default;

        // Assignment operator (deleted).
        IRendererEffect& operator =(const IRendererEffect&) = delete;

    public:
        // Called by renderer after applying per-scene information but before beginning any drawing.
        virtual void onStartPass(IRenderContext& context) const = 0;

        // Called by renderer after finishing drawing all objects in a pass.
        virtual void onFinishPass(IRenderContext& context) const = 0;

        // Called by renderer immediately before drawing triangles.
        virtual void onStartRenderObject(
            IRenderContext& context,
            unsigned int offset,
            unsigned int count) const = 0;
    };
}