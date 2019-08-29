#pragma once
#include <glm/glm.hpp>
#include <memory>
#include <map>
#include <string>

#include "Common\Error.h"
#include "Content\Materials\MaterialParameter.h"

namespace Daybreak
{
    /** Rendering path to use when rendering objects with this material. */
    enum class MaterialType : int
    {
        Traditional
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
        void setParameter(MaterialParameterType parameter, float value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameterType parameter, const glm::vec2& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameterType parameter, const glm::vec3& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameterType parameter, const glm::vec4& value);

        /** Set material parameter to value. */
        void setParameter(MaterialParameterType parameter, const material_texture_t& value);

        /** Check if parameter is defined. */
        bool isParameterDefined(MaterialParameterType parameter) const;

        /** Get parameter as float or throw exception if cannot cast or not defined. */
        float getFloatParameter(MaterialParameterType parameter) const;

        /** Get parameter as vec2 or throw exception if cannot cast or not defined. */
        glm::vec2 getVector2Parameter(MaterialParameterType parameter) const;

        /** Get parameter as vec3 or throw exception if cannot cast or not defined. */
        glm::vec3 getVector3Parameter(MaterialParameterType parameter) const;

        /** Get parameter as vec3 or throw exception if cannot cast or not defined. */
        glm::vec4 getVector4Parameter(MaterialParameterType parameter) const;

        /** Get parameter as material_texture_t or throw exception if cannot cast or not defined. */
        material_texture_t getTextureParameter(MaterialParameterType parameter) const;

        /** Delete a parameter from the material. */
        bool removeParameter(MaterialParameterType parameter);

        /** Get material class type. */
        MaterialType type() const noexcept { return m_type; }

        /** Set material class type. */
        void setType(MaterialType type) noexcept { m_type = type; }

    public:
        /** Material parameter was not defined exception. */
        class ParameterNotDefinedException : public DaybreakEngineException
        {
        public:
            ParameterNotDefinedException(const std::string& materialName, const std::string& parameterName);

            std::string materialName() const noexcept { return m_materialName; }
            std::string parameterName() const noexcept { return m_parameterName; }

            static std::string formatMessage(const std::string& materialName, const std::string& parameterName);

        private:
            std::string m_materialName;
            std::string m_parameterName;
        };

        /** Material parameter value could not be cast to requested type. */
        class ParameterCastException : public DaybreakEngineException
        {
        public:
            ParameterCastException(
                const std::string& materialName,
                const std::string& parameterName,
                const std::string& requestedType);

            std::string materialName() const noexcept { return m_materialName; }
            std::string parameterName() const noexcept { return m_parameterName; }
            std::string requestedType() const noexcept { return m_requestedType; }

            static std::string formatMessage(
                const std::string& materialName,
                const std::string& parameterName,
                const std::string& requestedType);

        private:
            std::string m_materialName;
            std::string m_parameterName;
            std::string m_requestedType;
        };

    private:
        MaterialType m_type;
        std::string m_name;
        std::map<MaterialParameterType, material_arg_value_t> m_parameters;
    };
}
