#pragma once
#include "Renderer/Phong/PhongLight.h"

#include <glm/glm.hpp>
#include <vector>

namespace Daybreak
{
    /// Represents a scene in a simulation.
    class Scene
    {
    public:
        Scene();
        ~Scene();
     
    // Lighting.
    public:
        // Get a directional light.
        DirectionalPhongLight directionalLight(_In_ size_t lightIndex) const;

        // Set a directional light.
        void setDirectionalLight(_In_ size_t lightIndex, _In_ const DirectionalPhongLight& light);

        // Get the number of active directional lights.
        size_t directionalLightCount() const noexcept { return m_directionalLights.size(); }

        // Set the number of active directional lights.
        void setDirectionalLightCount(_In_ size_t count);

        // Get the number of active point lights.
        size_t pointLightCount() const noexcept { return m_pointLights.size(); }

        // Set the number of active point lights.
        void setPointLightCount(_In_ size_t count);

        // Get a point light.
        PointPhongLight pointLight(_In_ size_t lightIndex) const;

        // Set a point light.
        void setPointLight(_In_ size_t lightIndex, _In_ const PointPhongLight& light);

        // Get the number of active spot lights.
        size_t spotLightCount() const noexcept { return m_spotLights.size(); }

        // Set the number of active spot lights.
        void setSpotLightCount(_In_ size_t count);

        // Get a spot light.
        SpotPhongLight spotLight(_In_ size_t lightIndex) const;

        // Set a spot light.
        void setSpotLight(_In_ size_t lightIndex, _In_ const SpotPhongLight& light);

    private:
        // TODO: Make lights be represented in the scene graph instead of a static set of lights.
        std::vector<DirectionalPhongLight> m_directionalLights;
        std::vector<PointPhongLight> m_pointLights;
        std::vector<SpotPhongLight> m_spotLights;
    };
}
