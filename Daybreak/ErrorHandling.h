#pragma once

#include <string>
#include <exception>

// Core error handling.
void glCheckForErrorsImpl(const char * filename, int line);

#ifdef _DEBUG
#define glCheckForErrors() glCheckForErrorsImpl(__FILE__, __LINE__);
#else
#define glCheckForErrors() 
#endif

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

/** Error when interacting with GLAD API. */
class GladException : public std::runtime_error
{
public:
    GladException(const std::string& message);
};

/** Error when interacting with OpneGL. */
class GlException : public std::runtime_error
{
public:
    /** Constructor. */
    GlException(
        unsigned int error,                 // OpenGL error enum value.
        const char* filename = nullptr,     // Optional name of source code file where error caught.
        int line = -1);                     // Optional line in source code file where error caught.

    /** Get the error code. */
    unsigned int ErrorCode() const { return m_error; }

    /** Get a human readable name for the error that was generated. */
    const char * ErrorName() const;

    /** Get the filename where the error was caught (Optional, might be null). */
    const char * FileName() const { return m_filename; }

    /** Get the line where the error was caught (Optional, might be -1). */
    int Line() const { return m_line; }

    /** Get a human readable name for an OpenGL error code. */
    static const char * GetErrorName(unsigned int error);

private:
    unsigned int m_error;
    const char * m_filename;
    int m_line;
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