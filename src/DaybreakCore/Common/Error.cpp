#include "stdafx.h"
#include "Error.h"

#include <string>
#include <sstream>
#include <exception>

//---------------------------------------------------------------------------------------------------------------------
DaybreakEngineException::DaybreakEngineException(const std::string& message, const std::string& details)
    : std::runtime_error(message),
      m_message(message),
      m_details(details)
{
}

//---------------------------------------------------------------------------------------------------------------------
RuntimeCheckException::RuntimeCheckException(
    const char * message,
    const char * expression,
    const char * file,
    unsigned int lineNumber)
    : DaybreakEngineException(
        message == nullptr ? "A runtime check has failed" : message,
        buildDetailsString(expression, file, lineNumber)),
      m_expression(expression),
      m_file(file),
      m_lineNumber(lineNumber)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string RuntimeCheckException::buildDetailsString(
    const char * expression,
    const char * file,
    unsigned int lineNumber)
{
    return std::string(expression) + "\n(" + file + ":" + std::to_string(lineNumber) + ")";
}

//---------------------------------------------------------------------------------------------------------------------
InvalidEnumerationValueException::InvalidEnumerationValueException(
    const char * typeName,
    int value,
    const char * file,
    unsigned int lineNumber)
    : RuntimeCheckException(
        "Enumeration had an unexpected or unhandled value",
        nullptr,
        file,
        lineNumber),
      m_typeName(typeName),
      m_value(value)
{
}

//---------------------------------------------------------------------------------------------------------------------
ContentReadException::ContentReadException(
    const std::string& contentFilePath,
    const std::string& contentTypeName,
    const std::string& errorMessage)
    : DaybreakEngineException(
        errorMessage,
        "Failed to load content type " + contentTypeName + " from file " + contentFilePath),
      m_filePath(contentFilePath),
      m_typeName(contentTypeName)
{
}

//---------------------------------------------------------------------------------------------------------------------
std::string DaybreakDataException::format(const std::string& message, const std::string& fileName, size_t lineNumber)
{
    if (fileName.size() == 0 && lineNumber == 0)
    {
        return message;
    }
    else 
    {
        std::stringstream ss;

        ss << message << " (";

        if (fileName.size() > 0)
        {
            ss << fileName;
        }

        if (lineNumber > 0)
        {
            ss << ":" << lineNumber;
        }

        ss << ")";

        return ss.str();
    }
    
}
