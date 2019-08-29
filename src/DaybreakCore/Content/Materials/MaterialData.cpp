#include "stdafx.h"
#include "MaterialData.h"

#include <stdexcept>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
MaterialData::MaterialData(const std::string& name, MaterialType type)
    : m_name(name),
      m_type(type)
{
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameterType parameter, float value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameterType parameter, const glm::vec2& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameterType parameter, const glm::vec3& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameterType parameter, const glm::vec4& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameterType parameter, const material_texture_t& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool MaterialData::isParameterDefined(MaterialParameterType parameter) const
{
    return m_parameters.find(parameter) != m_parameters.end();
}

//---------------------------------------------------------------------------------------------------------------------
float MaterialData::getFloatParameter(MaterialParameterType parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, to_string(parameter));
    }
    
    auto valuePointer = std::get_if<float>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, to_string(parameter), "float");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec2 MaterialData::getVector2Parameter(MaterialParameterType parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, to_string(parameter));
    }

    auto valuePointer = std::get_if<glm::vec2>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, to_string(parameter), "vec2");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 MaterialData::getVector3Parameter(MaterialParameterType parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, to_string(parameter));
    }

    auto valuePointer = std::get_if<glm::vec3>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, to_string(parameter), "vec3");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec4 MaterialData::getVector4Parameter(MaterialParameterType parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, to_string(parameter));
    }

    auto valuePointer = std::get_if<glm::vec4>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, to_string(parameter), "vec4");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
material_texture_t MaterialData::getTextureParameter(MaterialParameterType parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, to_string(parameter));
    }

    auto valuePointer = std::get_if<material_texture_t>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, to_string(parameter), "material_texture_t");
    }

    return *valuePointer;
}
