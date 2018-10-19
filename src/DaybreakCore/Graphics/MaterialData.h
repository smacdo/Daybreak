#pragma once
#include <variant>
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <string>

namespace Daybreak
{
    class IImage;
    typedef std::variant<float, glm::vec2, glm::vec3, glm::vec4, std::shared_ptr<IImage>> material_arg_value_t;

    enum class MaterialType
    {
        Traditional
    };

    enum class MaterialParameter
    {
        AmbientColor,
        DiffuseColor,
        DiffuseMap,
        SpecularColor,
        SpecularMap,
        Shininess,
        EmissiveMap
    };

    /** Data about a material loaded from disk. */
    class MaterialData
    {
    public:
        /** Constructor. */
        MaterialData(const std::string& name, MaterialType type);

        /** Get material name. */
        std::string name() const noexcept { return m_name; }

        /** Set material name. */
        void setName(const std::string& name) noexcept { m_name = name; }

        /** Set material parameter to value. */
        void setParameter(MaterialParameter parameter, float value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameter parameter, const glm::vec2& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameter parameter, const glm::vec3& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameter parameter, const glm::vec4& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameter parameter, std::shared_ptr<IImage> value);

        /** Check if parameter is defined. */
        bool isParameterDefined(MaterialParameter parameter);

        /** Get parameter as float or throw exception if cannot cast or not defined. */
        float getFloatParameter(MaterialParameter parameter) const;

        /** Get parameter as vec2 or throw exception if cannot cast or not defined. */
        glm::vec2 getVector2Parameter(MaterialParameter parameter) const;

        /** Get parameter as vec3 or throw exception if cannot cast or not defined. */
        glm::vec3 getVector3Parameter(MaterialParameter parameter) const;

        /** Get parameter as vec3 or throw exception if cannot cast or not defined. */
        glm::vec4 getVector4Parameter(MaterialParameter parameter) const;

        /** Get parameter as IImage or throw exception if cannot cast or not defined. */
        std::shared_ptr<IImage> getImageParameter(MaterialParameter parameter) const;

        /** Get material class type. */
        MaterialType type() const noexcept { return m_type; }

        /** Set material class type. */
        void setType(MaterialType type) noexcept { m_type = type; }

    private:
        MaterialType m_type;
        std::string m_name;
        std::map<MaterialParameter, material_arg_value_t> m_parameters;
    };
}
