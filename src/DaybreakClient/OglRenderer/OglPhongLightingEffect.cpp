#include "stdafx.h"
#include "OglPhongLightingEffect.h"
#include "OglShader.h"
#include "OglRenderContext.h"

#include "Renderer/Phong/PhongMaterial.h"
#include "Scene/Camera.h"

#include "Common/Error.h"

using namespace Daybreak;
using namespace Daybreak::OpenGlRenderer;

namespace PhongLightingEffectConstants
{
    size_t MaxDirectionalLightCount = 3;
    size_t MaxPointLightCount = 4;
    size_t MaxSpotLightCount = 1;
}

//---------------------------------------------------------------------------------------------------------------------
OglPhongLightingEffect::OglPhongLightingEffect(_In_ std::shared_ptr<OglShader> shader)
    : PhongLightingEffect(
        PhongLightingEffectConstants::MaxDirectionalLightCount,
        PhongLightingEffectConstants::MaxPointLightCount,
        PhongLightingEffectConstants::MaxSpotLightCount),
      m_shader(shader),
      m_directionalLightVars(PhongLightingEffectConstants::MaxDirectionalLightCount),
      m_pointLightVars(PhongLightingEffectConstants::MaxPointLightCount),
      m_spotLightVars(PhongLightingEffectConstants::MaxSpotLightCount)
{
    CHECK_NOT_NULL(shader);
    InitShaderVariables();
}

//---------------------------------------------------------------------------------------------------------------------
OglPhongLightingEffect::~OglPhongLightingEffect() = default;

//---------------------------------------------------------------------------------------------------------------------
void OglPhongLightingEffect::InitShaderVariables()
{
    m_viewMatrixShaderVar = m_shader->getVariable("view");
    m_viewPosShaderVar = m_shader->getVariable("viewPos");
    m_projectionShaderVar = m_shader->getVariable("projection");

    // Initialize material shader variables.
    m_materialVars.diffuseSampler = m_shader->getVariable("material.diffuse");
    m_materialVars.specularSampler = m_shader->getVariable("material.specular");
    m_materialVars.hasDiffuseTexture = m_shader->getVariable("material.hasDiffuseTexture");
    m_materialVars.hasSpecularTexture = m_shader->getVariable("material.hasSpecularTexture");
    m_materialVars.ambientColor = m_shader->getVariable("material.ambientColor");
    m_materialVars.diffuseColor = m_shader->getVariable("material.diffuseColor");
    m_materialVars.specularColor = m_shader->getVariable("material.specularColor");
    m_materialVars.shininess = m_shader->getVariable("material.shininess");

    // Initialize lighting variables (a lot!).
    m_directionalLightCountVar = m_shader->getVariable("directionalLightCount");
    m_pointLightCountVar = m_shader->getVariable("pointLightCount");

    for (size_t i = 0; i < maxDirectionalLightCount(); i++)
    {
        std::string index = std::to_string(i);

        m_directionalLightVars[i].direction = m_shader->getVariable("directionalLights[" + index + "].direction");
        m_directionalLightVars[i].ambient = m_shader->getVariable("directionalLights[" + index + "].ambient");
        m_directionalLightVars[i].diffuse = m_shader->getVariable("directionalLights[" + index + "].diffuse");
        m_directionalLightVars[i].specular = m_shader->getVariable("directionalLights[" + index + "].specular");
    }

    for (size_t i = 0; i < maxPointLightCount(); i++)
    {
        std::string index = std::to_string(i);

        m_pointLightVars[i].position = m_shader->getVariable("pointLights[" + index + "].position");
        m_pointLightVars[i].constant = m_shader->getVariable("pointLights[" + index + "].constant");
        m_pointLightVars[i].linear = m_shader->getVariable("pointLights[" + index + "].linear");
        m_pointLightVars[i].quadratic = m_shader->getVariable("pointLights[" + index + "].quadratic");
        m_pointLightVars[i].ambient = m_shader->getVariable("pointLights[" + index + "].ambient");
        m_pointLightVars[i].diffuse = m_shader->getVariable("pointLights[" + index + "].diffuse");
        m_pointLightVars[i].specular = m_shader->getVariable("pointLights[" + index + "].specular");
    }
}

//---------------------------------------------------------------------------------------------------------------------
void OglPhongLightingEffect::onStartPass(_In_ IRenderContext& context) const
{
    PhongLightingEffect::onStartPass(context);

    // Activate phong shader program.
    context.bindShader(m_shader);

    // Set camera shader parameters.
    CHECK_NOT_NULL(m_camera);

    context.setShaderMatrix4(m_viewMatrixShaderVar, m_camera->view());
    context.setShaderVector3f(m_viewPosShaderVar, m_camera->position());
    context.setShaderMatrix4(m_projectionShaderVar, m_camera->perspective());
}

//---------------------------------------------------------------------------------------------------------------------
void OglPhongLightingEffect::onFinishPass(_In_ IRenderContext& context) const
{
    PhongLightingEffect::onFinishPass(context);
}

//---------------------------------------------------------------------------------------------------------------------
void OglPhongLightingEffect::onStartRenderObject(
    _In_ IRenderContext& context,
    _In_ unsigned int offset,
    _In_ unsigned int count) const
{
    PhongLightingEffect::onStartRenderObject(context, offset, count);

    // Apply material parameters.
    if (m_materialDirty)
    {
        m_materialDirty = false;

        if (m_material->hasDiffuseTexture())
        {
            context.bindTexture(m_material->diffuseTexture(), 0);

            context.setShaderInt(m_materialVars.diffuseSampler, 0);
            context.setShaderBool(m_materialVars.hasDiffuseTexture, true);
        }
        else
        {
            context.setShaderBool(m_materialVars.hasDiffuseTexture, false);
            context.setShaderVector3f(m_materialVars.ambientColor, m_material->ambientColor());
            context.setShaderVector3f(m_materialVars.diffuseColor, m_material->diffuseColor());
        }

        if (m_material->hasSpecularTexture())
        {
            context.bindTexture(m_material->specularTexture(), 1);

            context.setShaderInt(m_materialVars.specularSampler, 1);
            context.setShaderBool(m_materialVars.hasSpecularTexture, true);
        }
        else
        {
            context.setShaderBool(m_materialVars.hasSpecularTexture, false);
            context.setShaderVector3f(m_materialVars.specularColor, m_material->specularColor());
        }

        context.setShaderFloat(m_materialVars.shininess, m_material->shininess());
    }
    
    // Apply lighting parameters.
    if (m_directionalLightsDirty)
    {
        m_directionalLightsDirty = false;

        context.setShaderInt(m_directionalLightCountVar, (int)m_directionalLightCount);

        for (size_t i = 0; i < m_directionalLightCount; ++i)
        {
            context.setShaderVector3f(m_directionalLightVars[i].direction, m_directionalLights[i].direction());
            context.setShaderVector3f(m_directionalLightVars[i].ambient, m_directionalLights[i].ambientColor());
            context.setShaderVector3f(m_directionalLightVars[i].diffuse, m_directionalLights[i].diffuseColor());
            context.setShaderVector3f(m_directionalLightVars[i].specular, m_directionalLights[i].specularColor());
        }
    }
    
    if (m_pointLightsDirty)
    {
        m_pointLightsDirty = false;

        context.setShaderInt(m_pointLightCountVar, (int)m_pointLightCount);

        for (size_t i = 0; i < m_pointLightCount; ++i)
        {
            context.setShaderVector3f(m_pointLightVars[i].position, m_pointLights[i].position());

            context.setShaderFloat(m_pointLightVars[i].constant, m_pointLights[i].constant());
            context.setShaderFloat(m_pointLightVars[i].linear, m_pointLights[i].linear());
            context.setShaderFloat(m_pointLightVars[i].quadratic, m_pointLights[i].quadratic());

            context.setShaderVector3f(m_pointLightVars[i].ambient, m_pointLights[i].ambientColor());
            context.setShaderVector3f(m_pointLightVars[i].diffuse, m_pointLights[i].diffuseColor());
            context.setShaderVector3f(m_pointLightVars[i].specular, m_pointLights[i].specularColor());
        }
    }
}
