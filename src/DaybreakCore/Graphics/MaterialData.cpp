#include "stdafx.h"
#include "MaterialData.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
MaterialData::MaterialData(const std::string& name)
    : m_name(name),
      m_type(MaterialType::Traditional)
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
void MaterialData::setParameter(MaterialParameter parameter, std::shared_ptr<IImage> value)
{
    m_parameters[parameter] = value;
}

//---------------------------------------------------------------------------------------------------------------------
bool MaterialData::isParameterDefined(MaterialParameter parameter)
{
    return m_parameters.find(parameter) != m_parameters.end();
}

//---------------------------------------------------------------------------------------------------------------------
float MaterialData::getFloatParameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        // TODO: Better exception.
        throw std::runtime_error("Material parameter not defined");
    }
    
    auto valuePointer = std::get_if<float>(&itr->second);

    if (valuePointer == nullptr)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Material parameter is not a float");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec2 MaterialData::getVector2Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        // TODO: Better exception.
        throw std::runtime_error("Material parameter not defined");
    }

    auto valuePointer = std::get_if<glm::vec2>(&itr->second);

    if (valuePointer == nullptr)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Material parameter is not a vec2");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec3 MaterialData::getVector3Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        // TODO: Better exception.
        throw std::runtime_error("Material parameter not defined");
    }

    auto valuePointer = std::get_if<glm::vec3>(&itr->second);

    if (valuePointer == nullptr)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Material parameter is not a vec3");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec4 MaterialData::getVector4Parameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        // TODO: Better exception.
        throw std::runtime_error("Material parameter not defined");
    }

    auto valuePointer = std::get_if<glm::vec4>(&itr->second);

    if (valuePointer == nullptr)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Material parameter is not a vec4");
    }

    return *valuePointer;
}

//---------------------------------------------------------------------------------------------------------------------
std::shared_ptr<IImage> MaterialData::getImageParameter(MaterialParameter parameter) const
{
    auto itr = m_parameters.find(parameter);

    if (itr == m_parameters.end())
    {
        // TODO: Better exception.
        throw std::runtime_error("Material parameter not defined");
    }

    auto valuePointer = std::get_if<std::shared_ptr<IImage>>(&itr->second);

    if (valuePointer == nullptr)
    {
        // TODO: Better exception.
        throw new std::runtime_error("Material parameter is not an image");
    }

    return *valuePointer;
}
