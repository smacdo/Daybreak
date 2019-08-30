#include "stdafx.h"
#include "RendererExceptions.h"

#include <string>
#include <sstream>
#include <exception>

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
ObjectNotShaderException::ObjectNotShaderException(unsigned int objectId, const std::string& shaderName)
    : DaybreakEngineException("Expected object id " + std::to_string(objectId) + " to be a shader", shaderName),
    m_shaderName(shaderName),
    m_objectId(objectId)
{
}

//---------------------------------------------------------------------------------------------------------------------
ObjectNotTextureException::ObjectNotTextureException(unsigned int objectId, const std::string& textureName)
    : DaybreakEngineException("Expected object id " + std::to_string(objectId) + " to be a texture", textureName),
    m_textureName(textureName),
    m_objectId(objectId)
{
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakShaderCompileException::DaybreakShaderCompileException(
    const std::string& shaderName,
    const std::string& compileInfo)
    : DaybreakEngineException("Failed to compile shader " + shaderName, compileInfo),
    m_shaderName(shaderName),
    m_compileInfo(compileInfo)
{
}

//---------------------------------------------------------------------------------------------------------------------
DaybreakShaderLinkException::DaybreakShaderLinkException(
    const std::string& shaderName,
    const std::string& linkInfo)
    : DaybreakEngineException("Failed to link shader " + shaderName, linkInfo),
    m_shaderName(shaderName),
    m_linkInfo(linkInfo)
{
}
