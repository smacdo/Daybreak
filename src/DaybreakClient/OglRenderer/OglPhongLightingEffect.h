#pragma once
#include "Renderer/Phong/PhongLightingEffect.h"
#include "Renderer/Phong/PhongEffectShaderVars.h"
#include "Renderer/ShaderVariable.h"

#include <glm/glm.hpp>
#include <memory>
#include <vector>

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
    private:
        // Called by renderer after applying per-scene information but before beginning any drawing.
        virtual void onStartPass(_In_ IRenderContext& context) const override;

        // Called by renderer after finishing drawing all objects in a pass.
        virtual void onFinishPass(_In_ IRenderContext& context) const override;

        //
        virtual void onStartRenderObject(
            _In_ IRenderContext& context,
            _In_ unsigned int offset,
            _In_ unsigned int count) const override;

    private:
        void InitShaderVariables();

    private:
        // Phong shader program.
        std::shared_ptr<OglShader> m_shader;

        // List of all shader variables for directional lights.
        std::vector<shader_directional_light_t> m_directionalLightVars;

        // List of all shader variables for point lights.
        std::vector<shader_point_light_t> m_pointLightVars;
       
        // List of all shader variables for spot lights.
        std::vector<shader_spot_light_t> m_spotLightVars;

        ShaderVariable m_viewMatrixShaderVar;
        ShaderVariable m_viewPosShaderVar;
        ShaderVariable m_projectionShaderVar;
        ShaderVariable m_directionalLightCountVar;
        ShaderVariable m_pointLightCountVar;

        shader_phong_material_t m_materialVars;
    };
}
