#pragma once
#include "Common/Error.h"

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
        void startPass(_In_ IRenderContext& context) const;

        // Called by renderer after finishing drawing all objects in a pass.
        void finishPass(_In_ IRenderContext& context) const;

        //
        void startRenderObject(
            _In_ IRenderContext& context,
            _In_ unsigned int offset,
            _In_ unsigned int count) const;

    // Effect event implementations.
    public:
        // Called by renderer after applying per-scene information but before beginning any drawing.
        virtual void onStartPass(_In_ IRenderContext& context) const;

        // Called by renderer after finishing drawing all objects in a pass.
        virtual void onFinishPass(_In_ IRenderContext& context) const;

        //
        virtual void onStartRenderObject(
            _In_ IRenderContext& context,
            _In_ unsigned int offset,
            _In_ unsigned int count) const;

    };
}