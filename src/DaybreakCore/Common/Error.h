#pragma once

#include <string>
#include <stdexcept>
#include <cassert>

// TODO: Restruct exceptions to have a secondary constructor that can take throw site information.

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
class RuntimeCheckException : public DaybreakEngineException
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
class InvalidEnumerationValueException : public RuntimeCheckException
{
public:
    /** Constructor. */
    InvalidEnumerationValueException(
        const char* typeName,
        int value,
        const char* file,
        unsigned int lineNumber);

    /** Get enumeration type name. */
    std::string TypeName() const { return m_typeName; }

    /** Get invalid value. */
    int Value() const { return m_value; }

private:
    const char* m_typeName;
    int m_value;
};

/** Data is not in the correct format or had unexpected data. */
class DaybreakDataException : public DaybreakEngineException
{
public:
    /** Constructor. */
    DaybreakDataException(const std::string& message)
        : DaybreakEngineException(message, "")
    {
    }

    /** Constructor. */
    DaybreakDataException(const std::string& message, const std::string& fileName, size_t lineNumber = 0)
        : DaybreakEngineException(format(message, fileName, lineNumber), ""),
          m_fileName(fileName),
          m_lineNumber(lineNumber)
    {
    }

    /** Get file name. */
    std::string fileName() const noexcept { return m_fileName; }

    /** Get if file name provided. */
    bool hasFileName() const noexcept { return m_fileName.size() > 0; }

    /** Get line number. */
    size_t lineNumber() const noexcept { return m_lineNumber; }

    /** Get if line number provided. */
    bool hasLineNumber() const noexcept { return m_lineNumber != 0; }

private:
    static std::string format(const std::string& message, const std::string& fileName, size_t lineNumber);

private:
    std::string m_fileName;
    size_t m_lineNumber = 0;
};

/** Exceptions with reading content after loading it into memory. */
class ContentReadException : public DaybreakEngineException
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
#define CHECK(x) if (!(x)) { throw RuntimeCheckException("Expected " ## #x " to be false", #x, __FILE__, __LINE__); }
#define CHECK_NOT_NULL(x) if ((x) == nullptr) { \
    throw RuntimeCheckException("Expected " ## #x " to not be null", #x ## " != nullptr", __FILE__, __LINE__); }
#define CHECK_NOT_ZERO(x) if ((x) == 0) { \
    throw RuntimeCheckException("Expected " ## #x " to not be zero", #x ## " != 0", __FILE__, __LINE__); }
#define CHECK_NOT_EMPTY(x) if ((x).empty()) { \
    throw RuntimeCheckException("Expected " ## #x " to not be empty", #x ## ".empty() == false", __FILE__, __LINE__); }
#define THROW_ENUM_SWITCH_NOT_HANDLED(enumType, value) \
    throw InvalidEnumerationValueException(#enumType, (int)(value), __FILE__, __LINE__)
