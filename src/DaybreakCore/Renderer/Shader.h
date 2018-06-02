#pragma once
#include "Renderer/BindableResource.h"
#include <glm/glm.hpp>
#include <string>

namespace Daybreak
{
    // Render API independent shader interface.
    class IShader : IBindableResource
    {
    public:
        // Destructor.
        virtual ~IShader() = default;

        // Set int shader parameter.
        virtual void setInt(const std::string& name, int value) const = 0;

        // Set float shader parameter.
        virtual void setFloat(const std::string& name, float value) const = 0;

        // Set bool shader parameter.
        virtual void setBool(const std::string& name, bool value) const = 0;

        // Set float2 shader parameter.
        virtual void setVector2f(const std::string& name, float x, float y) const = 0;

        // Set float2 shader parameter.
        virtual void setVector2f(const std::string& name, const glm::vec2& v) const = 0;

        // Set float3 shader parameter.
        virtual void setVector3f(const std::string& name, float x, float y, float z) const = 0;

        // Set float3 shader parameter.
        virtual void setVector3f(const std::string& name, const glm::vec3& v) const = 0;

        // Set float4 shader parameter.
        virtual void setVector4f(const std::string& name, float x, float y, float z, float w) const = 0;

        // Set float4 shader parameter.
        virtual void setVector4f(const std::string& name, const glm::vec4& v) const = 0;

        // Set matrix4x4 shader parameter.
        virtual void setMatrix3(const std::string& name, const glm::mat3& v) const = 0;

        // Set matrix4x4 shader parameter.
        virtual void setMatrix4(const std::string& name, const glm::mat4& v) const = 0;

        // Get name of shader.
        virtual std::string name() const = 0;
    };
}