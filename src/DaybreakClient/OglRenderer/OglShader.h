#pragma once
#include "Renderer/Shader.h"
#include <string>
#include <memory>

#include <glm\glm.hpp>
#include <glad\glad.h>

namespace Daybreak::OpenGlRenderer
{
    // OpenGL shader encapsulation.
    class OglShader final : public IShader
    {
    public:
        // Constructor.
        OglShader(GLuint shaderObjectId, const std::string& shaderName);

        // Destructor.
        virtual ~OglShader();

        // Activate shader.
        virtual void bind() override;

        // Set int shader parameter.
        virtual void setInt(const std::string& name, int value) const override;

        // Set float shader parameter.
        virtual void setFloat(const std::string& name, float value) const override;

        // Set bool shader parameter.
        virtual void setBool(const std::string& name, bool value) const override;

        // Set float2 shader parameter.
        virtual void setVector2f(const std::string& name, float x, float y) const override;

        // Set float2 shader parameter.
        virtual void setVector2f(const std::string& name, const glm::vec2& v) const override;

        // Set float3 shader parameter.
        virtual void setVector3f(const std::string& name, float x, float y, float z) const override;

        // Set float3 shader parameter.
        virtual void setVector3f(const std::string& name, const glm::vec3& v) const override;

        // Set float4 shader parameter.
        virtual void setVector4f(const std::string& name, float x, float y, float z, float w) const override;

        // Set float4 shader parameter.
        virtual void setVector4f(const std::string& name, const glm::vec4& v) const override;

        // Set matrix4x4 shader parameter.
        virtual void setMatrix3(const std::string& name, const glm::mat3& v) const override;

        // Set matrix4x4 shader parameter.
        virtual void setMatrix4(const std::string& name, const glm::mat4& v) const override;

        // Get name of shader.
        virtual std::string name() const override { return m_name; }

        // Get shader program object id.
        GLuint shaderProgram() const { return m_shaderProgram; }

        // Set shader program object.
        void setShaderProgram(GLuint id);

        // Load shader program from shader files on disk.
        static std::unique_ptr<OglShader> generateFromFile(
            const std::string& shaderProgramName,
            const std::string& vertexShaderPath,
            const std::string& fragmentShaderPath);

        // Load shader program from in memory strings.
        static std::unique_ptr<OglShader> generate(
            const std::string& shaderProgramName,
            const std::string& vertexShaderName,
            const std::string& vertexShaderString,
            const std::string& fragmentShaderName,
            const std::string& fragmentShaderString);

    private:
        // Load a file into a string and return it. Will throw an exception if failed to load.
        static std::string ReadTextFromFile(const std::string& filepath);

        // Checks that the shader was compiled successfully otherwise throws an exception.
        static void VerifyShaderCompiled(GLuint shader, const std::string& shaderName);

        // Checks that the shader was linked successfully otherwise throws an exception.
        static void VerifyShaderLinked(GLuint program, const std::string& programName);

    private:
        virtual void destroy();

    private:
        // Debug name for shader program.
        std::string m_name;

        // Shader object id.
        GLuint m_shaderProgram = 0;
    };
}
