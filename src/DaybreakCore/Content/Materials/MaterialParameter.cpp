#include "stdafx.h"
#include "MaterialParameter.h"

using namespace Daybreak;

//---------------------------------------------------------------------------------------------------------------------
std::string Daybreak::to_string(MaterialParameterType& param)
{
    switch (param)
    {
    case MaterialParameterType::AmbientColor:
        return "AmbientColor";
    case MaterialParameterType::DiffuseColor:
        return "DiffuseColor";
    case MaterialParameterType::DiffuseMap:
        return "DiffuseMap";
    case MaterialParameterType::SpecularColor:
        return "SpecularColor";
    case MaterialParameterType::SpecularMap:
        return "SpecularMap";
    case MaterialParameterType::Shininess:
        return "Shininess";
    case MaterialParameterType::Opacity:
        return "Opacity";
    case MaterialParameterType::EmissiveMap:
        return "EmissiveMap";
    case MaterialParameterType::NormalMap:
        return "NormalMap";
    case MaterialParameterType::DisplacementMap:
        return "DisplacementMap";
    default:
        assert(false && "MaterialParameterType enum case missing for to_string");
        return "UNKNOWN";
    }
}
