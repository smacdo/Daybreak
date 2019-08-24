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
void MaterialData::setParameter(MaterialParameter parameter, float value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameter parameter, const glm::vec2& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameter parameter, const glm::vec3& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameter parameter, const glm::vec4& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
void MaterialData::setParameter(MaterialParameter parameter, const material_texture_t& value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool MaterialData::isParameterDefined(MaterialParameter parameter) const
{
    return m_parameters.find(parameter) != m_parameters.end();
}

//---------------------------------------------------------------------------------------------------------------------
float MaterialData::getFloatParameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, "TODO ADD ENUM MACRO NAME");
    }
    
    auto valuePointer = std::get_if<float>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, "TODO ADD ENUM MACRO NAME", "float");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec2 MaterialData::getVector2Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, "TODO ADD ENUM MACRO NAME");
    }

    auto valuePointer = std::get_if<glm::vec2>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, "TODO ADD ENUM MACRO NAME", "vec2");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 MaterialData::getVector3Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, "TODO ADD ENUM MACRO NAME");
    }

    auto valuePointer = std::get_if<glm::vec3>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, "TODO ADD ENUM MACRO NAME", "vec3");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec4 MaterialData::getVector4Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, "TODO ADD ENUM MACRO NAME");
    }

    auto valuePointer = std::get_if<glm::vec4>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, "TODO ADD ENUM MACRO NAME", "vec4");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
material_texture_t MaterialData::getTextureParameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        throw ParameterNotDefinedException(m_name, "TODO ADD ENUM MACRO NAME");
    }

    auto valuePointer = std::get_if<material_texture_t>(&itr->second);

    if (valuePointer == nullptr)
    {
        throw ParameterCastException(m_name, "TODO ADD ENUM MACRO NAME", "material_texture_t");
    }

    return *valuePointer;
}
