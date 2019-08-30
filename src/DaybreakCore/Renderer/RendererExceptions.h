#pragma once

#include "Common\Error.h"

namespace Daybreak
{
    /** Object is not a shader exception. */
    class ObjectNotShaderException : public DaybreakEngineException
    {
    public:
        /** Constructor. */
        ObjectNotShaderException(unsigned int objectId, const std::string& shaderName);

        /** Get the name of the shader. */
        std::string ShaderName() const { return m_shaderName; }

        /** Get the object id. */
        unsigned int ObjectId() const { return m_objectId; }

    private:
        std::string m_shaderName;
        unsigned int m_objectId;
    };

    /** Object is not a texture exception. */
    class ObjectNotTextureException : public DaybreakEngineException
    {
    public:
        /** Constructor. */
        ObjectNotTextureException(unsigned int objectId, const std::string& textureName);

        /** Get the name of the shader. */
        std::string TextureName() const { return m_textureName; }

        /** Get the object id. */
        unsigned int ObjectId() const { return m_objectId; }

    private:
        std::string m_textureName;
        unsigned int m_objectId;
    };

    /** Shader compilation exception. */
    class DaybreakShaderCompileException : public DaybreakEngineException
    {
    public:
        /** Constructor. */
        DaybreakShaderCompileException(
            const std::string& shaderName,          /// Name of the shader (filename if loaded from disk).
            const std::string& compileInfo);        /// Messages outut from compiling the shader.

        /** Get the shader name. */
        std::string ShaderName() const { return m_shaderName; }

        /** Get the output from compiling the shader. */
        std::string CompileInfo() const { return m_compileInfo; }

    private:
        std::string m_shaderName;
        std::string m_compileInfo;
    };

    /** Shader program link exception. */
    class DaybreakShaderLinkException : public DaybreakEngineException
    {
    public:
        /** Constructor. */
        DaybreakShaderLinkException(
            const std::string& shaderName,          /// Name of the shader.
            const std::string& linkInfo);           /// Messages outut from compiling the shader.

                                                    /** Get the shader name. */
        std::string ShaderName() const { return m_shaderName; }

        /** Get the output from linking the shader program. */
        std::string CompileInfo() const { return m_linkInfo; }

    private:
        std::string m_shaderName;
        std::string m_linkInfo;
    };
}
