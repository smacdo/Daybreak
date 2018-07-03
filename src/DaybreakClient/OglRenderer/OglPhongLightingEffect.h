#pragma once
#include "Renderer/Phong/PhongLightingEffect.h"
#include "Renderer/Phong/PhongEffectShaderVars.h"
#include "Renderer/ShaderVariable.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

namespace Daybreak
{
    class Camera;
    class PhongMaterial;
    class DirectionalPhongLight;
    class PointPhongLight;
    class SpotPhongLight;
}

namespace Daybreak::OpenGlRenderer
{
    class OglShader;

    // Standard phong lighting effect.
    class OglPhongLightingEffect : public Daybreak::PhongLightingEffect
    {
    public:
        // Constructor.
        OglPhongLightingEffect(_In_ const std::shared_ptr<OglShader> phongShader);

        // Destructor.
        virtual ~OglPhongLightingEffect();

    // Effect events.
    public:
        // Called by renderer after applying per-scene information but before beginning any drawing.
        virtual void onStartPass(IRenderContext& context) const override;

        // Called by renderer after finishing drawing all objects in a pass.
        virtual void onFinishPass(IRenderContext& context) const override;

        //
        virtual void onStartRenderObject(
            IRenderContext& context,
            unsigned int offset,
            unsigned int count) const override;

    // Effect parameters.
    public:
        // Set the camera view.
        void setCamera(_In_ std::shared_ptr<const Daybreak::Camera> camera);

        // Set the active material.
        void setMaterial(_In_ std::shared_ptr<const Daybreak::PhongMaterial> material);

    // Lighting.
    public:
        // Set a directional light.
        void setDirectionalLight(_In_ size_t lightIndex, _In_ const Daybreak::DirectionalPhongLight& light);

        // Get the number of active directional lights.
        size_t directionalLightCount() const noexcept { return m_directionalLightCount; }

        // Set the number of active directional lights.
        void setDirectionalLightCount(_In_ size_t count);

        // Get the number of active point lights.
        size_t pointLightCount() const noexcept { return m_pointLightCount; }

        // Set the number of active point lights.
        void setPointLightCount(_In_ size_t count);

        // Set a point light.
        void setPointLight(_In_ size_t lightIndex, _In_ const Daybreak::PointPhongLight& light);

        // Get the number of active spot lights.
        size_t spotLightCount() const noexcept { return m_spotLightCount; }

        // Set the number of active spot lights.
        void setSpotLightCount(_In_ size_t count);

        // Set a spot light.
        void setSpotLight(_In_ size_t lightIndex, _In_ const Daybreak::SpotPhongLight& light);

    private:
        void InitShaderVariables();

    private:
        // Phong shader program.
        std::shared_ptr<OglShader> m_shader;

        // State caching flags for optimization.
        mutable bool m_materialChanged = false;
        mutable bool m_directionalLightsChanged = false;
        mutable bool m_pointLightsChanged = false;
        mutable bool m_spotLightsChanged = false;

        // Number of active directional lights.
        size_t m_directionalLightCount = 0;

        // Number of active point lights.
        size_t m_pointLightCount = 0;

        // Number of active spot lights.
        size_t m_spotLightCount = 0;

        // List of all directional lights.
        std::vector<DirectionalPhongLight> m_directionalLights;

        // List of all shader variables for directional lights.
        std::vector<shader_directional_light_t> m_directionalLightVars;

        // List of all point lights.
        std::vector<PointPhongLight> m_pointLights;

        // List of all shader variables for point lights.
        std::vector<shader_point_light_t> m_pointLightVars;

        // List of all spot lights.
        std::vector<SpotPhongLight> m_spotLights;

        // List of all shader variables for spot lights.
        std::vector<shader_spot_light_t> m_spotLightVars;

        ShaderVariable m_viewMatrixShaderVar;
        ShaderVariable m_viewPosShaderVar;
        ShaderVariable m_projectionShaderVar;
        ShaderVariable m_directionalLightCountVar;
        ShaderVariable m_pointLightCountVar;

        std::shared_ptr<const Daybreak::PhongMaterial> m_material;
        shader_phong_material_t m_materialVars;

        std::shared_ptr<const Daybreak::Camera> m_camera;
    };
}