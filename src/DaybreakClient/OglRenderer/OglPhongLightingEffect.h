#pragma once
#include "Renderer/Phong/PhongLight.h"
#include <glm/vec3.hpp>
#include <vector>

namespace Daybreak::OpenGlRenderer
{
    // Phong lighting shader program interface.
    class OglPhongLightingEffect
    {
    public:
        // Constructor.
        OglPhongLightingEffect(size_t maxDirectionalLights, size_t maxPointLights, size_t maxSpotLights);

        // Destructor.
        virtual ~OglPhongLightingEffect();

    private:
        size_t m_maxDirectionalLightCount = 0;
        size_t m_maxPointLightCount = 0;
        size_t m_maxSpotLightCount = 0;
        std::vector<DirectionalPhongLight> m_directionalLights;
        std::vector<PointPhongLight> m_pointLights;
        std::vector<SpotPhongLight> m_spotLights;
    };
}