#include "stdafx.h"
#include "MaterialData.h"

#include <stdexcept>
#include <sstream>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
MaterialData::ParameterNotDefinedException::ParameterNotDefinedException(
    const std::string& materialName,
    const std::string& parameterName)
    : DaybreakEngineException("Material parameter not defined", formatMessage(materialName, parameterName)),
      m_materialName(materialName),
      m_parameterName(parameterName)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string MaterialData::ParameterNotDefinedException::formatMessage(
    const std::string& materialName,
    const std::string& parameterName)
{
    std::stringstream ss;
    ss << "Material '" << materialName << "' does not have a parameter named '" + parameterName + "'";
    return ss.str();
}

//---------------------------------------------------------------------------------------------------------------------
MaterialData::ParameterCastException::ParameterCastException(
    const std::string& materialName,
    const std::string& parameterName,
    const std::string& requestedType)
    : DaybreakEngineException(
        "Could not cast material parameter to requested type",
        formatMessage(materialName, parameterName, requestedType)),
    m_materialName(materialName),
    m_parameterName(parameterName)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string MaterialData::ParameterCastException::formatMessage(
    const std::string& materialName,
    const std::string& parameterName,
    const std::string& requestedType)
{
    std::stringstream ss;
    ss
        << "Could not cast parameter '" << parameterName
        << "' on material ''" << materialName
        << "' to type '" << requestedType << "'";
    return ss.str();
}
