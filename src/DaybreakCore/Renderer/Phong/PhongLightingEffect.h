#pragma once
#include "Renderer/Texture.h"
#include <glm/vec3.hpp>
#include <vector>
#include "PhongLight.h"

namespace Daybreak
{
    // Phong lighting shader program interface.
    class PhongLightingEffect
    {
    public:
        // Constructor.
        PhongLightingEffect(size_t maxDirectionalLights, size_t maxPointLights, size_t maxSpotLights);

        // Destructor.
        virtual ~PhongLightingEffect();

    private:
        size_t m_maxDirectionalLightCount = 0;
        size_t m_maxPointLightCount = 0;
        size_t m_maxSpotLightCount = 0;
        std::vector<DirectionalPhongLight> m_directionalLights;
        std::vector<PointPhongLight> m_pointLights;
        std::vector<SpotPhongLight> m_spotLights;
    };
}