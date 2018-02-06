#pragma once
#include <string>
#include <memory>

#include <glm\glm.hpp>

/** Encapsulates loading and configuring an OpenGL shader. */
class Shader
{
public:
    /** Constructor. */
    Shader(const std::string& shaderName, unsigned int shaderObjectId);

    /** Copy constructor deleted because copying OpenGL shader programs is not supported. */
    Shader(const Shader&) = delete;

    /** Destructor. */
    ~Shader();

    /** Assignment deleted because copying OpenGL shader programs is not supported. */
    Shader& operator=(const Shader&) = delete;

    /** Activate shader for use. */
    void Activate();

    /** Set int shader parameter. */
    void SetInt(const std::string& name, int value) const;

    /** Set float shader parameter. */
    void SetFloat(const std::string& name, float value) const;

    /** Set bool shader parameter. */
    void SetBool(const std::string& name, bool value) const;

    /** Set float2 shader parameter. */
    void SetVector2f(const std::string& name, float x, float y) const;

    /** Set float2 shader parameter. */
    void SetVector2f(const std::string& name, const glm::vec2& v) const;

    /** Set float3 shader parameter. */
    void SetVector3f(const std::string& name, float x, float y, float z) const;

    /** Set float3 shader parameter. */
    void SetVector3f(const std::string& name, const glm::vec3& v) const;

    /** Set float4 shader parameter. */
    void SetVector4f(const std::string& name, float x, float y, float z, float w) const;

    /** Set float4 shader parameter. */
    void SetVector4f(const std::string& name, const glm::vec4& v) const;

    /** Set matrix4x4 shader parameter. */
    void SetMatrix4(const std::string& name, const glm::mat4& v) const;

    /** Get name of shader. */
    std::string Name() const { return m_name; }

    /** Get shader program object id. */
    unsigned int ShaderProgramId() const { return m_shaderProgram; }

    /** Load shader program from shader files on disk. */
    static std::unique_ptr<Shader> LoadFromFile(
        const std::string& shaderProgramName,
        const std::string& vertexShaderPath,
        const std::string& fragmentShaderPath);

    /** Load shader program from in memory strings. */
    static std::unique_ptr<Shader> LoadFromString(
        const std::string& shaderProgramName,
        const std::string& vertexShaderName,
        const std::string& vertexShaderString,
        const std::string& fragmentShaderName,
        const std::string& fragmentShaderString);

private:
    /** Load a file into a string and return it. Will throw an exception if failed to load. */
    static std::string ReadTextFromFile(const std::string& filepath);

    /** Checks that the shader was compiled successfully otherwise throws an exception. */
    static void VerifyShaderCompiled(unsigned int shader, const std::string& shaderName);

    /** Checks that the shader was linked successfully otherwise throws an exception. */
    static void VerifyShaderLinked(unsigned int program, const std::string& programName);

private:
    /** Debug name for shader program. */
    std::string m_name;

    /** Shader object id. */
    unsigned int m_shaderProgram;
};

