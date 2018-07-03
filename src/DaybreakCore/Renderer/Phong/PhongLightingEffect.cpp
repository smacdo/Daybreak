#include "stdafx.h"
#include "PhongLightingEffect.h"
#include "Renderer/RenderContext.h"
#include "Renderer/Phong/PhongMaterial.h"
#include "Scene/Camera.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
PhongLightingEffect::PhongLightingEffect(
    _In_ size_t maxDirectionalLights,
    _In_ size_t maxPointLights,
    _In_ size_t maxSpotLights)
    : m_directionalLights(maxDirectionalLights),
      m_pointLights(maxPointLights),
      m_spotLights(maxSpotLights)
{
}

//---------------------------------------------------------------------------------------------------------------------
PhongLightingEffect::~PhongLightingEffect() = default;

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::onStartPass(_In_ IRenderContext& context) const
{
    m_materialDirty = true;
    m_directionalLightsDirty = true;
    m_pointLightsDirty = true;
    m_spotLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setCamera(_In_ std::shared_ptr<const Camera> camera)
{
    m_camera = camera;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setMaterial(_In_ std::shared_ptr<const PhongMaterial> material)
{
    m_material = material;
    m_materialDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setDirectionalLight(_In_ size_t lightIndex, _In_ const DirectionalPhongLight& light)
{
    if (lightIndex >= directionalLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many directional lights");
    }

    m_directionalLights[lightIndex] = light;
    m_directionalLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setPointLight(_In_ size_t lightIndex, _In_ const PointPhongLight& light)
{
    if (lightIndex >= pointLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many point lights");
    }

    m_pointLights[lightIndex] = light;
    m_pointLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setSpotLight(_In_ size_t lightIndex, _In_ const Daybreak::SpotPhongLight& light)
{
    if (lightIndex >= spotLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many spot lights");
    }

    m_spotLights[lightIndex] = light;
    m_spotLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setDirectionalLightCount(_In_ size_t count)
{
    if (count > m_directionalLights.size())
    {
        // TODO: Better exception.
        throw std::runtime_error("Too many directional lights");
    }

    m_directionalLightCount = count;
    m_directionalLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setPointLightCount(_In_ size_t count)
{
    if (count > m_pointLights.size())
    {
        // TODO: Better exception.
        throw std::runtime_error("Too many point lights");
    }

    m_pointLightCount = count;
    m_spotLightsDirty = true;
}

//---------------------------------------------------------------------------------------------------------------------
void PhongLightingEffect::setSpotLightCount(_In_ size_t count)
{
    if (count > m_spotLights.size())
    {
        // TODO: Better exception.
        throw std::runtime_error("Too many spot lights");
    }

    m_spotLightCount = count;
    m_spotLightsDirty = true;
}
