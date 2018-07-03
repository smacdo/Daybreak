#pragma once
#include "Renderer/RendererEffect.h"
#include "Renderer/Phong/PhongLight.h"

#include <vector>

namespace Daybreak
{
    // Phong lighting shader program interface.
    class PhongLightingEffect : public IRendererEffect
    {
    public:
        // Constructor.
        PhongLightingEffect() = default;

        // Destructor.
        virtual ~PhongLightingEffect() = default;

    private:
    };
}