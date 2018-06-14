#include "stdafx.h"
#include "OglPhongLightingEffect.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

//---------------------------------------------------------------------------------------------------------------------
OglPhongLightingEffect::OglPhongLightingEffect(
    size_t maxDirectionalLights,
    size_t maxPointLights,
    size_t maxSpotLights)
    : m_maxDirectionalLightCount(maxDirectionalLights),
      m_maxPointLightCount(m_maxPointLightCount),
      m_maxSpotLightCount(m_maxSpotLightCount)
{
    m_directionalLights.reserve(maxDirectionalLights);
    m_pointLights.reserve(maxPointLights);
    m_spotLights.reserve(maxSpotLights);
}

//---------------------------------------------------------------------------------------------------------------------
OglPhongLightingEffect::~OglPhongLightingEffect() = default;

