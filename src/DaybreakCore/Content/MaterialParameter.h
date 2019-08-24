#pragma once
#include <variant>
#include <glm/glm.hpp>
#include <memory>
#include <string>

#include "app\support\enumtable.h"

namespace Daybreak
{
    class IImage;

    /** References a texture file on disk, and may or may not be loaded. */
    struct material_texture_t
    {
        std::string filepath;
        std::shared_ptr<IImage> image;
    };

    /** A material parameter. */
    enum class MaterialParameter : int
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

    /** Holds a material parameter value. */
    typedef std::variant<float, glm::vec2, glm::vec3, glm::vec4, material_texture_t> material_arg_value_t;
}
