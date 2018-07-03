#pragma once
#include "Renderer/ShaderVariable.h"

namespace Daybreak
{
    struct shader_phong_material_t
    {
        ShaderVariable diffuseSampler;
        ShaderVariable hasDiffuseTexture;
        ShaderVariable specularSampler;
        ShaderVariable hasSpecularTexture;
        ShaderVariable ambientColor;
        ShaderVariable diffuseColor;
        ShaderVariable specularColor;
        ShaderVariable shininess;
    };

    struct shader_directional_light_t
    {
        ShaderVariable direction;
        ShaderVariable ambient;
        ShaderVariable diffuse;
        ShaderVariable specular;
    };

    struct shader_point_light_t
    {
        ShaderVariable position;
        ShaderVariable direction;
        ShaderVariable constant;
        ShaderVariable linear;
        ShaderVariable quadratic;
        ShaderVariable ambient;
        ShaderVariable diffuse;
        ShaderVariable specular;
    };

    struct shader_spot_light_t
    {
        ShaderVariable position;
        ShaderVariable direction;
        ShaderVariable ambient;
        ShaderVariable diffuse;
        ShaderVariable specular;
    };
}