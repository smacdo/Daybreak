#pragma once

#include <string>
#include <exception>
#include <cassert>

void DisplayErrorDialog(
    const std::string& title,
    const std::string& message);

/** Error when interacting with SDL API. */
class SdlException : public std::runtime_error
{
public:
    SdlException(const std::string& message);
    
    std::string Message() const { return m_message; }
    static std::string GetLastSdlError();

private:
    std::string m_message;
};

/** Core engine errors. */
class DaybreakEngineException : public std::runtime_error
{
public:
    /** Constructor. */
    DaybreakEngineException(const std::string& message, const std::string& details);

    /** Get the (short) message explaining why the exception was thrown. */
    std::string Message() const { return m_message; }

    /** Get an optional longer message explaining details behind the exception. */
    std::string Details() const { return m_details; }

private:
    std::string m_message;
    std::string m_details;
};

/** Core engine errors. */
class RuntimeCheckException : DaybreakEngineException
{
public:
    /** Constructor. */
    RuntimeCheckException(
        const char * message,
        const char * expression,
        const char * file,
        unsigned int lineNumber);

    std::string expression() const { return m_expression; }
    std::string file() const { return m_file; }
    unsigned int lineNumber() const { return m_lineNumber; }

    static std::string buildDetailsString(
        const char * expression,
        const char * file,
        unsigned int lineNumber);

private:
    std::string m_expression = "";
    std::string m_file = "";
    unsigned int m_lineNumber = 0;
};

/** Invalid or unsupported enumeration value. */
class InvalidEnumerationValueException : public DaybreakEngineException
{
public:
    /** Constructor. */
    InvalidEnumerationValueException(const std::string& enumTypeName, int value);

    /** Get enumeration type name. */
    std::string EnumTypeName() const { return m_enumTypeName; }

    /** Get invalid value. */
    int InvalidValue() const { return m_value; }

private:
    std::string m_enumTypeName;
    int m_value;
};

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

/** Exceptions with reading content after loading it into memory. */
class ContentReadException : DaybreakEngineException
{
public:
    /** Constructor. */
    ContentReadException(
        const std::string& contentFilePath,
        const std::string& contentTypeName,
        const std::string& errorMessage);
    
    /** Get the content file path. */
    std::string FilePath() const { return m_filePath; }

    /** Get the content type name. */
    std::string TypeName() const { return m_typeName; }

private:
    std::string m_filePath;
    std::string m_typeName;
};

//---------------------------------------------------------------------------------------------------------------------
// Runtime check macros.
//---------------------------------------------------------------------------------------------------------------------
#define EXPECT(x, msg) assert(x && msg)
#define CHECK_NOT_NULL(x) if ((x) == nullptr) { \
    throw RuntimeCheckException("Expected " ## #x " to not be null", #x ## " != nullptr", __FILE__, __LINE__); }
#define CHECK_NOT_ZERO(x) if ((x) == 0) { \
    throw RuntimeCheckException("Expected " ## #x " to not be zero", #x ## " != 0", __FILE__, __LINE__); }
#define CHECK_NOT_EMPTY(x) if ((x).empty()) { \
    throw RuntimeCheckException("Expected " ## #x " to not be empty", #x ## ".empty() == false", __FILE__, __LINE__); }
