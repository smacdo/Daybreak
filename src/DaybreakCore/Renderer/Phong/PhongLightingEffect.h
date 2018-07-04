#pragma once
#include "Renderer/RendererEffect.h"
#include "Renderer/Phong/PhongLight.h"

#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace Daybreak
{
    class Camera;
    class PhongMaterial;

    // Render independent phong lighting effect.
    class PhongLightingEffect : public IRendererEffect
    {
    public:
        // Constructor.
        PhongLightingEffect(
            _In_ size_t maxDirectionalLights,
            _In_ size_t maxPointLights,
            _In_ size_t maxSpotLights);

        // Destructor.
        virtual ~PhongLightingEffect();

    // Effect parameters.
    public:
        // Set the camera view.
        void setCamera(_In_ std::shared_ptr<const Camera> camera);

        // Set the active material.
        void setMaterial(_In_ std::shared_ptr<const PhongMaterial> material);

        // Set model matrix.
        void setModelMatrix(_In_ const glm::mat4& model);

        // Set normal matrix.
        void setNormalMatrix(_In_ const glm::mat3& normal);

    // Lighting.
    public:
        // Set a directional light.
        void setDirectionalLight(_In_ size_t lightIndex, _In_ const DirectionalPhongLight& light);

        // Get the number of active directional lights.
        size_t directionalLightCount() const noexcept { return m_directionalLightCount; }

        // Set the number of active directional lights.
        void setDirectionalLightCount(_In_ size_t count);

        // Get the maximum number of directional lights.
        size_t maxDirectionalLightCount() const noexcept { return m_directionalLights.size(); }

        // Get the number of active point lights.
        size_t pointLightCount() const noexcept { return m_pointLightCount; }

        // Set the number of active point lights.
        void setPointLightCount(_In_ size_t count);

        // Get the maximum number of point lights.
        size_t maxPointLightCount() const noexcept { return m_pointLights.size(); }

        // Set a point light.
        void setPointLight(_In_ size_t lightIndex, _In_ const PointPhongLight& light);

        // Get the number of active spot lights.
        size_t spotLightCount() const noexcept { return m_spotLightCount; }

        // Set the number of active spot lights.
        void setSpotLightCount(_In_ size_t count);

        // Get the maximum number of spot lights.
        size_t maxSpotLightCount() const noexcept { return m_spotLights.size(); }

        // Set a spot light.
        void setSpotLight(_In_ size_t lightIndex, _In_ const SpotPhongLight& light);

    // Effect events.
    public:
        // Called by renderer after applying per-scene information but before beginning any drawing.
        virtual void onStartPass(_In_ IRenderContext& context) const override;

    protected:
        // Get if the material was changed.
        bool materialDirty() const noexcept { return m_materialDirty; }

        // Set if the material was changed.
        void setMaterialDirty(_In_ bool isDirty) const noexcept { m_materialDirty = false; }

        // Get if directional lights were changed.
        bool directionalLightsDirty() const noexcept { return m_materialDirty; }

        // Set if directional lights were changed.
        void setDirectionalLightsDirty(_In_ bool isDirty) const noexcept { m_directionalLightsDirty = isDirty; }

        // Get if point lights were changed.
        bool pointLightsDirty() const noexcept { return m_pointLightsDirty; }

        // Set if point lights were changed.
        void setPointLightsDirty(_In_ bool isDirty) const noexcept { m_pointLightsDirty = isDirty; }

        // Get if spot lights were changed.
        bool spotLightsDirty() const noexcept { return m_spotLightsDirty; }

        // Set if spot lights were changed.
        void setSpotLightsDirty(_In_ bool isDirty) const noexcept { m_spotLightsDirty = isDirty; }

    protected:
        // State caching flags for optimization.
        mutable bool m_materialDirty = false;
        mutable bool m_directionalLightsDirty = false;
        mutable bool m_pointLightsDirty = false;
        mutable bool m_spotLightsDirty = false;

        // Number of active directional lights.
        size_t m_directionalLightCount = 0;

        // Number of active point lights.
        size_t m_pointLightCount = 0;

        // Number of active spot lights.
        size_t m_spotLightCount = 0;

        // List of all directional lights.
        std::vector<DirectionalPhongLight> m_directionalLights;

        // List of all point lights.
        std::vector<PointPhongLight> m_pointLights;

        // List of all spot lights.
        std::vector<SpotPhongLight> m_spotLights;

        // Active material.
        std::shared_ptr<const PhongMaterial> m_material;

        // Active camera.
        std::shared_ptr<const Camera> m_camera;

        // Active model matrix.
        glm::mat4 m_modelMatrix;

        // Active normal matrix.
        glm::mat3 m_normalMatrix;
    };
}