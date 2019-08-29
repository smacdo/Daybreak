#pragma once
#include <variant>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "Renderer\Texture.h"

namespace Daybreak
{
    /** A material parameter. (Update MaterialData.cpp to_string when adding/removing). */
    enum class MaterialParameterType : int
    {
        AmbientColor,
        DiffuseColor,
        DiffuseMap,
        SpecularColor,
        SpecularMap,
        Shininess,
        Opacity,                   ///< 0 is fully transparent, 1 is opaque.
        EmissiveMap,
        NormalMap,
        DisplacementMap
    };

    /** Convert MaterialParameter to string. */
    std::string to_string(MaterialParameterType& param);

    /** References a texture file on disk, and may or may not be loaded. */
    struct material_texture_t
    {
        std::string filepath;
        std::shared_ptr<ITexture2d> texture;
        TextureParameters textureParams;
    };

    /** Holds a material parameter value. */
    typedef std::variant<float, glm::vec2, glm::vec3, glm::vec4, material_texture_t> material_arg_value_t;
}
