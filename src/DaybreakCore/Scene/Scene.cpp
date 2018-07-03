#include "stdafx.h"
#include "Scene.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
Scene::Scene() = default;

//---------------------------------------------------------------------------------------------------------------------
Scene::~Scene() = default;

//---------------------------------------------------------------------------------------------------------------------
DirectionalPhongLight Scene::directionalLight(_In_ size_t lightIndex) const
{
    if (lightIndex >= directionalLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many directional lights");
    }

    return m_directionalLights[lightIndex];
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setDirectionalLight(
    _In_ size_t lightIndex,
    _In_ const DirectionalPhongLight& light)
{
    if (lightIndex >= directionalLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many directional lights");
    }

    m_directionalLights[lightIndex] = light;
}

//---------------------------------------------------------------------------------------------------------------------
PointPhongLight Scene::pointLight(_In_ size_t lightIndex) const
{
    if (lightIndex >= pointLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many point lights");
    }

    return m_pointLights[lightIndex];
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setPointLight(
    _In_ size_t lightIndex,
    _In_ const PointPhongLight& light)
{
    if (lightIndex >= pointLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many point lights");
    }

    m_pointLights[lightIndex] = light;
}

//---------------------------------------------------------------------------------------------------------------------
SpotPhongLight Scene::spotLight(_In_ size_t lightIndex) const
{
    if (lightIndex >= spotLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many spot lights");
    }

    return m_spotLights[lightIndex];
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setSpotLight(
    _In_ size_t lightIndex,
    _In_ const SpotPhongLight& light)
{
    if (lightIndex >= spotLightCount())
    {
        // TODO: Better exception.
        throw new std::runtime_error("Too many spot lights");
    }

    m_spotLights[lightIndex] = light;
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setDirectionalLightCount(_In_ size_t count)
{
    m_directionalLights.resize(count);
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setPointLightCount(_In_ size_t count)
{
    m_pointLights.resize(count);
}

//---------------------------------------------------------------------------------------------------------------------
void Scene::setSpotLightCount(_In_ size_t count)
{
    m_spotLights.resize(count);
}
