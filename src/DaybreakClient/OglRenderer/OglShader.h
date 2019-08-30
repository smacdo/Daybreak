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

        // Get shader uniform address.
        virtual ShaderVariable getVariable(const std::string& name) const override;

        // Get name of shader.
        virtual std::string name() const override { return m_name; }

        // Get shader program object id.
        GLuint shaderProgram() const { return m_shaderProgram; }

        // Set shader program object.
        void setShaderProgram(GLuint id);

        // Load shader program from in memory strings.
        static std::unique_ptr<OglShader> generate(
            const std::string& shaderProgramName,
            const std::string_view& vertexShaderString,
            const std::string_view& fragmentShaderString);

    private:
        // Checks that the shader was compiled successfully otherwise throws an exception.
        static void VerifyShaderCompiled(
            GLuint shader,
            const std::string& programName,
            const std::string& stageName);

        // Checks that the shader was linked successfully otherwise throws an exception.
        static void VerifyShaderLinked(
            GLuint program,
            const std::string& programName);

    private:
        virtual void destroy();

    private:
        // Debug name for shader program.
        std::string m_name;

        // Shader object id.
        GLuint m_shaderProgram = 0;
    };
}
