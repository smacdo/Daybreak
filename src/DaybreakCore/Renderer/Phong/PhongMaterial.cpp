#include "stdafx.h"
#include "PhongMaterial.h"
#include "Content\Materials\MaterialData.h"

#include "Common/Error.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
PhongMaterial::PhongMaterial(const MaterialData& data)
{
    if (data.type() != MaterialType::Traditional)
    {
        throw DaybreakEngineException("PhongMaterial can only be created from traditional materials", data.name());
    }

    if (data.isParameterDefined(MaterialParameterType::AmbientColor))
    {
        m_ambientColor = data.getVector3Parameter(MaterialParameterType::AmbientColor);
    }

    if (data.isParameterDefined(MaterialParameterType::DiffuseColor))
    {
        m_diffuseColor = data.getVector3Parameter(MaterialParameterType::DiffuseColor);
    }

    if (data.isParameterDefined(MaterialParameterType::SpecularColor))
    {
        m_specularColor = data.getVector3Parameter(MaterialParameterType::SpecularColor);
    }

    if (data.isParameterDefined(MaterialParameterType::Shininess))
    {
        m_shininess = data.getFloatParameter(MaterialParameterType::Shininess);
    }

    if (data.isParameterDefined(MaterialParameterType::DiffuseMap))
    {
        m_diffuseTexture = data.getTextureParameter(MaterialParameterType::DiffuseMap).texture;
    }
    
    if (data.isParameterDefined(MaterialParameterType::SpecularMap))
    {
        m_specularTexture = data.getTextureParameter(MaterialParameterType::SpecularMap).texture;
    }

    if (data.isParameterDefined(MaterialParameterType::EmissiveMap))
    {
        m_emissiveTexture = data.getTextureParameter(MaterialParameterType::EmissiveMap).texture;
    }
}
